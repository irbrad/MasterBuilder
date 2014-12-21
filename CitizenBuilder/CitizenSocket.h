//
//  CitizenSocket.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__CitizenSocket__
#define __MasterBuilder__CitizenSocket__

#include <stdint.h>
#include <google/protobuf/message.h>
#include <dispatch/dispatch.h>
#include <thread>
#include <deque>

class CitizenSocket
{
public:
    static CitizenSocket* instance;

    CitizenSocket();
    ~CitizenSocket();

    /**
     *  Initialize the client socket
     */
    void InitializeSocket();

    /**
     *  Send a message to the server
     *
     *  @param msg     proto buffer message
     *  @param msgType unsigned message type
     */
    void SendMsg( google::protobuf::Message* msg, uint32_t msgType );

    /**
     *  Figure out the number of pending messages queued up. Requires locking.
     *
     *  @return pending msg count
     */
    size_t PendingMsgCount();

    /**
     *  Get a message from the deque
     *
     *  @return a pending message
     */
    class MsgBase* PopMessage();

private:
    int32_t Socket;

    dispatch_queue_t HighPriorityQueue;
    dispatch_source_t SocketSource;

    std::mutex Mutex;
    std::deque< class MsgBase* > PendingMessages;
};

#endif /* defined(__MasterBuilder__CitizenSocket__) */
