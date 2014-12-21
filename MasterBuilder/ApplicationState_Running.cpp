//
//  ApplicationState_Running.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "ApplicationState_Running.h"

#include "MasterBuilderApplication.h"
#include "CitizenDB.h"
#include "MasterSocket.h"
#include "Messaging.h"

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
    if ( MasterBuilderApplication::Socket && MasterBuilderApplication::Socket->PendingMsgCount() )
    {
        MsgBase* result = MasterBuilderApplication::Socket->PopMessage();
        if ( !result )
        {
            return;
        }

        switch ( result->type() )
        {
            case MsgBase_MsgId_Register:
            {
                if ( Register* regMsg = Utils::CreateMsgSubclass< Register >( result->subclass() ) )
                {
                    MasterBuilderApplication::DB->Register( regMsg->host(), regMsg->corecount(),
                                                            regMsg->priority() );
                    delete regMsg;
                }
                break;
            }
            case MsgBase_MsgId_Unregister:
            {
                if ( Unregister* unregMsg =
                         Utils::CreateMsgSubclass< Unregister >( result->subclass() ) )
                {
                    MasterBuilderApplication::DB->Release( unregMsg->host() );
                    delete unregMsg;
                }
                break;
            }
            case MsgBase_MsgId_Load:
            {
                if ( Load* loadMsg = Utils::CreateMsgSubclass< Load >( result->subclass() ) )
                {
                    delete loadMsg;
                }
                break;
            }
            case MsgBase_MsgId_RequestCPU:
            {
                if ( RequestCPU* requestMsg =
                         Utils::CreateMsgSubclass< RequestCPU >( result->subclass() ) )
                {
                    delete requestMsg;
                }
                break;
            }
            case MsgBase_MsgId_ReleaseCPU:
            {
                if ( ReleaseCPU* releaseMsg =
                         Utils::CreateMsgSubclass< ReleaseCPU >( result->subclass() ) )
                {
                    delete releaseMsg;
                }
                break;
            }
            case MsgBase_MsgId_Ping:
            {
                if ( Ping* pingMsg = Utils::CreateMsgSubclass< Ping >( result->subclass() ) )
                {
                    delete pingMsg;
                }
                break;
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