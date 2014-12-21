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

#include "Messages.pb.h"

namespace
{
/**
 *  Parse the peeked buffer value to determine header size
 *
 *  @param buffer 4 bytes
 *
 *  @return the calculated header size
 */
google::protobuf::uint32 ParseHeader( const char* buffer )
{
    google::protobuf::uint32 size;
    google::protobuf::io::ArrayInputStream ais( buffer, 4 );

    google::protobuf::io::CodedInputStream input( &ais );
    input.ReadVarint32( &size );
    return size;
}

/**
 *  Allocates a message of type T
 *
 *  @param Data a string that we can parse containing our message body
 *
 *  @return pointer of type T that has been constructed using Data
 */
template < typename T >
T* MsgSubclass( const std::string& Data )
{
    T* result = new T;
    result->ParseFromString( Data );
    return result;
}

/**
 *  Parse the state of our master socket and return a pointer representing a
 *  message
 *
 *  @param Socket the socket we're operating on
 *  @param Size   the computed header size
 *
 *  @return a child of Message based on the type on the wire
 */
google::protobuf::Message* ParseMessage( int32_t Socket,
                                         google::protobuf::uint32 Size )
{
    ssize_t bytecount = 0;
    char buffer[ Size + 4 ];

    if ( ( bytecount = recv( Socket, buffer, Size + 4, MSG_WAITALL ) ) < 0 )
    {
        fprintf( stderr, "Parse failed (errno = %d)", errno );
        return nullptr;
    }

    google::protobuf::io::ArrayInputStream ais( buffer, Size + 4 );
    google::protobuf::io::CodedInputStream input( &ais );

    input.ReadVarint32( &Size );

    google::protobuf::io::CodedInputStream::Limit limit =
        input.PushLimit( Size );

    MsgBase msg;
    msg.ParseFromCodedStream( &input );

    google::protobuf::Message* result = nullptr;

    switch ( msg.type() )
    {
    case MsgBase_MsgId_Register:
        result = MsgSubclass< Register >( msg.subclass() );
        break;
    case MsgBase_MsgId_Unregister:
        result = MsgSubclass< Unregister >( msg.subclass() );
        break;
    case MsgBase_MsgId_Load:
        result = MsgSubclass< Load >( msg.subclass() );
        break;
    case MsgBase_MsgId_RequestCPU:
        result = MsgSubclass< RequestCPU >( msg.subclass() );
        break;
    case MsgBase_MsgId_ReleaseCPU:
        result = MsgSubclass< ReleaseCPU >( msg.subclass() );
        break;
    case MsgBase_MsgId_Ping:
        result = MsgSubclass< Ping >( msg.subclass() );
        break;
    case MsgBase_MsgId_Invalid:
    default: break;
    }

    input.PopLimit( limit );

    return result;
}
}

/*static*/ MasterSocket* MasterSocket::instance = nullptr;

MasterSocket::MasterSocket()
{
    Socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( Socket < 0 )
    {
        fprintf( stderr, "Failed to open socket (errno = %d)\n", errno );
        close( Socket );
        return;
    }

    fprintf( stderr, "We opened our socket, id = %d\n", Socket );

    if ( setsockopt( Socket, SOL_SOCKET, SO_REUSEADDR, "1", 4 ) < 0 )
    {
        fprintf( stderr, "Failed to set socket options (errno = %d)\n", errno );
        close( Socket );
        return;
    }

    if ( setsockopt( Socket, SOL_SOCKET, SO_KEEPALIVE, "1", 4 ) < 0 )
    {
        fprintf( stderr, "Failed to set socket options (errno = %d)\n", errno );
        close( Socket );
        return;
    }

    struct sockaddr_in sin;
    memset( &sin, 0, sizeof( sin ) );

    sin.sin_len = sizeof( sin );
    sin.sin_family = AF_INET;
    sin.sin_port = htons( 1337 );
    sin.sin_addr.s_addr = INADDR_ANY;

    if ( bind( Socket, reinterpret_cast< struct sockaddr* >( &sin ),
               sizeof( sin ) ) < 0 )
    {
        fprintf( stderr, "Failed to bind socket (errno = %d)\n", errno );
        close( Socket );
        return;
    }

    if ( listen( Socket, 10 ) < 0 )
    {
        fprintf( stderr, "Failed to listen to socket (errno = %d)\n", errno );
        close( Socket );
        return;
    }

    HighPriorityQueue =
        dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 );

    DispatchSource = dispatch_source_create( DISPATCH_SOURCE_TYPE_READ, Socket,
                                             0, HighPriorityQueue );

    dispatch_source_set_event_handler( DispatchSource, ^{
        int32_t temp_sock = -1;

        struct sockaddr_in sin;
        memset( &sin, 0, sizeof( sin ) );
        sin.sin_family = AF_INET;
        socklen_t len = sizeof( sin );

        if ( ( temp_sock =
                   accept( Socket, reinterpret_cast< struct sockaddr* >( &sin ),
                           &len ) ) < 0 )
        {
            fprintf( stderr, "Accept failed (errno = %d)\n", errno );
            return;
        }
        
#if DEBUG
        fprintf( stderr, "Connecting IP Address = '%s'\n",
                 inet_ntoa( sin.sin_addr ) );
#endif

        char buffer[ 4 ] = {0};
        ssize_t bytecount = 0;

        if ( ( bytecount = recv( temp_sock, buffer, 4, MSG_PEEK ) ) < 0 )
        {
            fprintf( stderr, "Error reading from socket (errno = %d)\n",
                     errno );
            return;
        }

        google::protobuf::Message* msg =
            ParseMessage( temp_sock, ParseHeader( buffer ) );

        if ( msg )
        { // I suppose we could just use .Lock / .Unlock but whatever
            std::lock_guard< std::mutex > guard( Mutex );
            PendingMessages.push_back( msg );
        }

        if ( msg )
        {
            fprintf( stderr, "dumping stuff \n %s \n",
                     msg->DebugString().c_str() );
        }
    } );

    dispatch_resume( DispatchSource );
}

MasterSocket::~MasterSocket()
{
    close( Socket );
}

size_t MasterSocket::PendingMsgCount()
{
    std::lock_guard< std::mutex > guard( Mutex );
    return PendingMessages.size();
}

google::protobuf::Message* MasterSocket::PopMessage()
{
    std::lock_guard< std::mutex > guard( Mutex );
    auto* msg = PendingMessages.front();
    PendingMessages.pop_front();

    return msg;
}
