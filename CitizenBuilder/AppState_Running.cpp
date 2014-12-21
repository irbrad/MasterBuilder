//
//  AppState_Running.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "AppState_Running.h"

#include <signal.h>
#include <stdio.h>

namespace
{
    static AppState_Running* sInstance = nullptr;
}

void AppState_Running::Update()
{
}

void AppState_Running::OnEnterState( EApplicationState previousState )
{
#if DEBUG
    fprintf( stderr, "Entering State = %s\n", GetStateName() );
#endif

    sInstance = this;
    struct sigaction new_action, old_action;

    new_action.sa_handler = AppState_Running::SignalHandler;
    sigemptyset( &new_action.sa_mask );
    new_action.sa_flags = 0;

    sigaction( SIGINT, NULL, &old_action );
    if ( old_action.sa_handler != SIG_IGN )
    {
        sigaction( SIGINT, &new_action, NULL );
    }
}

void AppState_Running::OnExitState( EApplicationState nextState )
{
#if DEBUG
    fprintf( stderr, "Exiting State = %s\n", GetStateName() );
#endif

    sInstance = nullptr;
}

/*static*/ void AppState_Running::SignalHandler( int32_t id )
{
    if ( sInstance )
    {
        sInstance->NextAppState = EApplicationState::Shutdown;
    }
}