//
//  Messaging.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/21/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "Messaging.h"

#include "Messages.pb.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>

namespace Utils
{
    google::protobuf::uint32 ParseHeader( const char* buffer )
    {
        google::protobuf::uint32 size;
        google::protobuf::io::ArrayInputStream ais( buffer, 4 );

        google::protobuf::io::CodedInputStream input( &ais );
        input.ReadVarint32( &size );
        return size;
    }

    void ParseMessage( int32_t Socket, google::protobuf::uint32 Size, MsgBase* Result )
    {
        ssize_t bytecount = 0;
        char buffer[ Size + 4 ];

        if ( ( bytecount = recv( Socket, buffer, Size + 4, MSG_WAITALL ) ) < 0 )
        {
            fprintf( stderr, "Parse failed (errno = %d)", errno );
            return;
        }

        google::protobuf::io::ArrayInputStream ais( buffer, Size + 4 );
        google::protobuf::io::CodedInputStream input( &ais );

        input.ReadVarint32( &Size );

        google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit( Size );

        Result->ParseFromCodedStream( &input );

        input.PopLimit( limit );
    }
}
