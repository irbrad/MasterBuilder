//
//  CitizenSocket.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__CitizenSocket__
#define __MasterBuilder__CitizenSocket__

#ifdef __APPLE__
	#define	USE_GCD		1
#else
	#define USE_GCD		0
#endif // __APPLE__


#include <stdint.h>
#include <google/protobuf/message.h>
#if USE_GCD
	#include <dispatch/dispatch.h>
#endif // USE_GCD
#include <thread>
#include <deque>
#include <mutex>

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

#if USE_GCD
    dispatch_queue_t HighPriorityQueue;
    dispatch_source_t SocketSource;
#endif // USE_GCD

    std::mutex Mutex;
    std::deque< class MsgBase* > PendingMessages;
};

#endif /* defined(__MasterBuilder__CitizenSocket__) */
