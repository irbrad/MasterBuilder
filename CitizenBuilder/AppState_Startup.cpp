//
//  AppState_Startup.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "AppState_Startup.h"

#include <thread>

#include "CitizenSocket.h"

#include "Messages.pb.h"

void AppState_Startup::Update()
{
    NextAppState = EApplicationState::Running;
}

void AppState_Startup::OnEnterState( EApplicationState previousState )
{
#if DEBUG
    fprintf( stderr, "Entering State = %s\n", GetStateName() );
#endif

    if ( !CitizenSocket::instance )
    {
        CitizenSocket::instance = new CitizenSocket;
    }

    if ( CitizenSocket::instance )
    {
        Register regMsg;
        regMsg.set_host( "127.0.0.1" );
        regMsg.set_corecount( std::thread::hardware_concurrency() );
        regMsg.set_priority( 10 );

        CitizenSocket::instance->SendMsg( &regMsg, MsgBase_MsgId_Register );
    }
}

void AppState_Startup::OnExitState( EApplicationState nextState )
{
#if DEBUG
    fprintf( stderr, "Exiting State = %s\n", GetStateName() );
#endif
}