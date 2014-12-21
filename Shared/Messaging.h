//
//  Messaging.h
//  MasterBuilder
//
//  Created by Sammy James on 12/21/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef MasterBuilder_Messaging_h
#define MasterBuilder_Messaging_h

#include <stdint.h>
#include <google/protobuf/message.h>

class MsgBase;

namespace Utils
{
    /**
     *  Allocates a message of type T
     *
     *  @param Data a string that we can parse containing our message body
     *
     *  @return pointer of type T that has been constructed using Data
     */
    template < typename T >
    T* CreateMsgSubclass( const std::string& Data )
    {
        T* result = new T;
        result->ParseFromString( Data );
        return result;
    }

    /**
     *  Parse the peeked buffer value to determine header size
     *
     *  @param buffer 4 bytes
     *
     *  @return the calculated header size
     */
    google::protobuf::uint32 ParseHeader( const char* buffer );

    /**
     *  Parse the state of our master socket and return a pointer representing a
     *  message
     *
     *  @param Socket the socket we're operating on
     *  @param Size   the computed header size
     *  @param Result place to store the parsed message
     *
     *  @return a child of Message based on the type on the wire
     */
    void ParseMessage( int32_t Socket, google::protobuf::uint32 Size, MsgBase* Result );
}

#endif
