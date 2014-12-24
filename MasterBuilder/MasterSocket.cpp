//
//  MasterSocket.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "MasterSocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "CitizenDB.h"
#include "Messaging.h"

#include "Messages.pb.h"

MasterSocket::MasterSocket()
{
#if USE_GCD
    HighPriorityQueue = dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 );
#endif // USE_GCD

    InitializeSocket();
    InitializeTick();
}

MasterSocket::~MasterSocket()
{
    close( Socket );

#if USE_GCD
    dispatch_source_cancel( SocketSource );
    dispatch_source_cancel( TimerSource );
#endif // USE_GCD
}

void MasterSocket::InitializeSocket()
{
    Socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( Socket < 0 )
    {
        fprintf( stderr, "Failed to open socket (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

    fprintf( stderr, "We opened our socket, id = %d\n", Socket );

    if ( setsockopt( Socket, SOL_SOCKET, SO_REUSEADDR, "1", 4 ) < 0 )
    {
        fprintf( stderr, "Failed to set socket options (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

    if ( setsockopt( Socket, SOL_SOCKET, SO_KEEPALIVE, "1", 4 ) < 0 )
    {
        fprintf( stderr, "Failed to set socket options (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

    struct sockaddr_in sin;
    memset( &sin, 0, sizeof( sin ) );

#ifdef SIN_LEN	// this field is BSD-specific
    sin.sin_len = sizeof( sin );
#endif // SIN_LEN
    sin.sin_family = AF_INET;
    sin.sin_port = htons( 1337 );
    sin.sin_addr.s_addr = INADDR_ANY;

    if ( bind( Socket, reinterpret_cast< struct sockaddr* >( &sin ), sizeof( sin ) ) < 0 )
    {
        fprintf( stderr, "Failed to bind socket (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

    if ( listen( Socket, 10 ) < 0 )
    {
        fprintf( stderr, "Failed to listen to socket (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

#if USE_GCD
    SocketSource =
        dispatch_source_create( DISPATCH_SOURCE_TYPE_READ, Socket, 0, HighPriorityQueue );

    dispatch_source_set_event_handler( SocketSource, ^{
        int32_t temp_sock = -1;

        struct sockaddr_in sin;
        memset( &sin, 0, sizeof( sin ) );
        sin.sin_family = AF_INET;
        socklen_t len = sizeof( sin );

        if ( ( temp_sock = accept( Socket, reinterpret_cast< struct sockaddr* >( &sin ), &len ) ) <
             0 )
        {
            fprintf( stderr, "Accept failed (error = %s)\n", strerror( errno ) );
            return;
        }

        {
            std::string host = inet_ntoa( sin.sin_addr );
            
#if DEBUG
            fprintf( stderr, "Connecting IP Address = '%s'\n", host.c_str() );
#endif

            std::lock_guard< std::mutex > guard( SocketMutex );
            ConnectedSockets.insert( std::make_pair( host, temp_sock ) );
        }

        char buffer[ 4 ] = {0};
        ssize_t bytecount = 0;

        if ( ( bytecount = recv( temp_sock, buffer, 4, MSG_PEEK ) ) < 0 )
        {
            fprintf( stderr, "Error reading from socket (error = %s)\n", strerror( errno ) );
            return;
        }

        MsgBase* msg = new MsgBase;
        Utils::ParseMessage( temp_sock, Utils::ParseHeader( buffer ), msg );

        if ( msg )
        { // I suppose we could just use .Lock / .Unlock but whatever
            std::lock_guard< std::mutex > guard( MessageMutex );
            PendingMessages.push_back( msg );
        }

        if ( msg )
        {
            fprintf( stderr, "dumping stuff \n %s \n", msg->DebugString().c_str() );
        }
    } );

    dispatch_resume( SocketSource );
#else

	// TODO: implement

#endif // USE_GCD
}

void MasterSocket::InitializeTick()
{
#if USE_GCD
    TimerSource = dispatch_source_create( DISPATCH_SOURCE_TYPE_TIMER, 0, 0, HighPriorityQueue );

    if ( TimerSource )
    {
        dispatch_source_set_timer( TimerSource,
                                   dispatch_time( DISPATCH_TIME_NOW, 60 * NSEC_PER_SEC ),
                                   60 * NSEC_PER_SEC, ( 1ull * NSEC_PER_SEC ) / 10 );

        dispatch_source_set_event_handler( TimerSource, ^{
            std::lock_guard< std::mutex > guard( SocketMutex );
            if ( ConnectedSockets.empty() )
            {
                return;
            }

            std::string encoded;
            Ping pinger;
            pinger.SerializeToString( &encoded );

            MsgBase wrapper;
            wrapper.set_type( MsgBase_MsgId_Ping );
            wrapper.set_subclass( encoded );

            char* buffer = new char[ wrapper.ByteSize() + 4 ];
            google::protobuf::io::ArrayOutputStream aos( buffer, wrapper.ByteSize() + 4 );
            google::protobuf::io::CodedOutputStream out( &aos );

            out.WriteVarint32( wrapper.ByteSize() );

            wrapper.SerializeToCodedStream( &out );

            for ( auto it = ConnectedSockets.cbegin(); it != ConnectedSockets.cend(); ++it )
            {
                if ( send( it->second, buffer, wrapper.ByteSize() + 4, 0 ) < 0 )
                {
                    fprintf( stderr, "Failed to send packet to '%s' (error = %s)\n",
                             it->first.c_str(), strerror( errno ) );

                    ConnectedSockets.erase( it->first );
                    break;
                }
            }

            delete[] buffer;
        } );

        dispatch_resume( TimerSource );
    }

#else

	// TODO: implement

#endif // USE_GCD
}

size_t MasterSocket::PendingMsgCount()
{
    std::lock_guard< std::mutex > guard( MessageMutex );
    return PendingMessages.size();
}

MsgBase* MasterSocket::PopMessage()
{
    std::lock_guard< std::mutex > guard( MessageMutex );
    auto* msg = PendingMessages.front();
    PendingMessages.pop_front();

    return msg;
}
