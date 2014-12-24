//
//  CitizenSocket.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "CitizenSocket.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "Messaging.h"

#include "Messages.pb.h"

/*static*/ CitizenSocket* CitizenSocket::instance = nullptr;

CitizenSocket::CitizenSocket()
{
#if USE_GCD
    HighPriorityQueue = dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 );
#endif // USE_GCD

    InitializeSocket();
}

CitizenSocket::~CitizenSocket()
{
    shutdown( Socket, SHUT_RDWR );
#if USE_GCD
    dispatch_source_cancel( SocketSource );
#endif // USE_GCD
}

void CitizenSocket::InitializeSocket()
{
    Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( Socket < 0 )
    {
        fprintf( stderr, "Failed to open socket (error = %s)\n", strerror( errno ) );
        close( Socket );
        return;
    }

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

    sin.sin_family = AF_INET;
    sin.sin_port = htons( 1337 );

    memset( &( sin.sin_zero ), 0, 8 );
    sin.sin_addr.s_addr = inet_addr( "127.0.0.1" );

    if ( connect( Socket, reinterpret_cast< struct sockaddr* >( &sin ), sizeof( sin ) ) < 0 )
    {
        fprintf( stderr, "Failed to connect socket (errno = %d)\n", errno );
        close( Socket );
        return;
    }

#if USE_GCD
    SocketSource =
        dispatch_source_create( DISPATCH_SOURCE_TYPE_READ, Socket, 0, HighPriorityQueue );

    dispatch_source_set_event_handler( SocketSource, ^{

        char buffer[ 4 ] = {0};
        ssize_t bytecount = 0;

        if ( ( bytecount = recv( Socket, buffer, 4, MSG_PEEK ) ) < 0 )
        {
            fprintf( stderr, "Error reading from socket (error = %s)\n", strerror( errno ) );
            return;
        }

        MsgBase* msg = new MsgBase;
        Utils::ParseMessage( Socket, Utils::ParseHeader( buffer ), msg );

        if ( msg )
        { // I suppose we could just use .Lock / .Unlock but whatever
            std::lock_guard< std::mutex > guard( Mutex );
            PendingMessages.push_back( msg );
        }

    } );

    dispatch_resume( SocketSource );
#else

	// TODO: implement

#endif // USE_GCD

}

void CitizenSocket::SendMsg( google::protobuf::Message* msg, uint32_t msgType )
{
#if DEBUG
    fprintf( stderr, "Sending Message... \n%s", msg->DebugString().c_str() );
#endif

    std::string output;
    msg->SerializeToString( &output );

    MsgBase wrapper;
    wrapper.set_type( (MsgBase_MsgId)msgType );
    wrapper.set_subclass( output );

    char* buffer = new char[ wrapper.ByteSize() + 4 ];
    google::protobuf::io::ArrayOutputStream aos( buffer, wrapper.ByteSize() + 4 );
    google::protobuf::io::CodedOutputStream out( &aos );

    out.WriteVarint32( wrapper.ByteSize() );

    wrapper.SerializeToCodedStream( &out );

    if ( send( Socket, buffer, wrapper.ByteSize() + 4, 0 ) < 0 )
    {
        fprintf( stderr, "Failed to send packet (error = %s)", strerror( errno ) );
    }

    delete[] buffer;
}

size_t CitizenSocket::PendingMsgCount()
{
    std::lock_guard< std::mutex > guard( Mutex );
    return PendingMessages.size();
}

MsgBase* CitizenSocket::PopMessage()
{
    std::lock_guard< std::mutex > guard( Mutex );
    MsgBase* result = PendingMessages.front();
    PendingMessages.pop_front();

    return result;
}
