//
//  MasterSocket.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__MasterSocket__
#define __MasterBuilder__MasterSocket__

#include <google/protobuf/message.h>
#include <dispatch/dispatch.h>
#include <thread>
#include <deque>

/**
 *  This is our master socket, or the thing we're listening to all of the
 *  slaves on. They will report their status and request information from us
 *  using this socket.
 */
class MasterSocket
{
public:
    static MasterSocket* instance;

    MasterSocket();
    ~MasterSocket();

    /**
     *  Determine the number of pending messages we've got stored up from
     *  our socket, these accrue from another thread. This operation must lock.
     *
     *  @return the number of pending messages we have
     */
    size_t PendingMsgCount();

    /**
     *  Pop a pending message, locking operation
     *
     *  @return a pointer to a unprocessed message
     */
    google::protobuf::Message* PopMessage();

private:
    int32_t Socket;
    dispatch_source_t DispatchSource;
    dispatch_queue_t HighPriorityQueue;

    std::mutex Mutex;
    std::deque< google::protobuf::Message* > PendingMessages;
};

#endif /* defined(__MasterBuilder__MasterSocket__) */
