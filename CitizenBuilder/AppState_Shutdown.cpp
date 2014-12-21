//
//  AppState_Shutdown.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "AppState_Shutdown.h"

#include "CitizenSocket.h"

#include "Messages.pb.h"

void AppState_Shutdown::Update()
{
}

void AppState_Shutdown::OnEnterState( EApplicationState previousState )
{
#if DEBUG
    fprintf( stderr, "Entering State = %s\n", GetStateName() );
#endif

    if ( CitizenSocket::instance )
    {
        Unregister msg;
        msg.set_host( "127.0.0.1" );
        CitizenSocket::instance->SendMsg( &msg, MsgBase_MsgId_Unregister );

        delete CitizenSocket::instance;
        CitizenSocket::instance = nullptr;
    }

    NextAppState = EApplicationState::Invalid;
}

void AppState_Shutdown::OnExitState( EApplicationState nextState )
{
#if DEBUG
    fprintf( stderr, "Exiting State = %s\n", GetStateName() );
#endif
}
