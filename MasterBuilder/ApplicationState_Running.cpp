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
        MsgBase* result = MasterSocket::instance->PopMessage();
        if ( !result )
        {
            return;
        }

        switch ( result->type() )
        {
            case MsgBase_MsgId_Register:
            {
                if ( Register* regMsg =
                         MasterSocket::CreateMsgSubclass< Register >( result->subclass() ) )
                {
                    CitizenDB::instance->Register( regMsg->host(), regMsg->corecount(),
                                                   regMsg->priority() );
                    delete regMsg;
                }
                break;
            }
            case MsgBase_MsgId_Unregister:
            {
                if ( Unregister* unregMsg =
                         MasterSocket::CreateMsgSubclass< Unregister >( result->subclass() ) )
                {
                    CitizenDB::instance->Release( unregMsg->host() );
                    delete unregMsg;
                }
                break;
            }
            case MsgBase_MsgId_Load:
            {
                if ( Load* loadMsg = MasterSocket::CreateMsgSubclass< Load >( result->subclass() ) )
                {
                    delete loadMsg;
                }
                break;
            }
            case MsgBase_MsgId_RequestCPU:
            {
                if ( RequestCPU* requestMsg =
                         MasterSocket::CreateMsgSubclass< RequestCPU >( result->subclass() ) )
                {
                    delete requestMsg;
                }
                break;
            }
            case MsgBase_MsgId_ReleaseCPU:
            {
                if ( ReleaseCPU* releaseMsg =
                         MasterSocket::CreateMsgSubclass< ReleaseCPU >( result->subclass() ) )
                {
                    delete releaseMsg;
                }
                break;
            }
            case MsgBase_MsgId_Ping:
            {
                if ( Ping* pingMsg = MasterSocket::CreateMsgSubclass< Ping >( result->subclass() ) )
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