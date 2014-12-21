//
//  ApplicationState_Running.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "ApplicationState_Running.h"

#include "CitizenDB.h"
#include "MasterSocket.h"

#include "Messages.pb.h"

ApplicationState_Running::ApplicationState_Running()
{
}

EApplicationState ApplicationState_Running::NextState() const
{
    return EApplicationState::Running;
}

void ApplicationState_Running::Update()
{
    if ( MasterSocket::instance && MasterSocket::instance->PendingMsgCount() )
    {
        google::protobuf::Message* result =
            MasterSocket::instance->PopMessage();

        if ( result )
        {
            // I feel kind of bad about this Dynamic Cast chain, but whatever.
            if ( Load* msg = dynamic_cast< Load* >( result ) )
            {
            }
            else if ( RequestCPU* msg = dynamic_cast< RequestCPU* >( result ) )
            {
                std::string host = "";
                uint8_t core = 0;
                if ( CitizenDB::instance->RequestCore( host, core ) )
                {
                }
            }
            else if ( ReleaseCPU* msg = dynamic_cast< ReleaseCPU* >( result ) )
            {
                CitizenDB::instance->ReleaseCore( msg->host(), msg->core() );
            }
            else if ( Register* msg = dynamic_cast< Register* >( result ) )
            {
                CitizenDB::instance->Register( msg->host(), msg->corecount(),
                                               msg->priority() );
            }
            else if ( Unregister* msg = dynamic_cast< Unregister* >( result ) )
            {
                CitizenDB::instance->Release( msg->host() );
            }
        }
    }
}

void ApplicationState_Running::OnEnterState( EApplicationState previousState )
{
}

void ApplicationState_Running::OnExitState( EApplicationState nextState )
{
}