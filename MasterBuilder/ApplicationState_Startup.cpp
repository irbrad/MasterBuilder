//
//  ApplicationState_Startup.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "ApplicationState_Startup.h"

#include "MasterBuilderApplication.h"
#include "CitizenDB.h"
#include "MasterSocket.h"

ApplicationState_Startup::ApplicationState_Startup()
    : NextAppState( EApplicationState::Startup )
{
}

void ApplicationState_Startup::Update()
{
    NextAppState = EApplicationState::Running;
}

void ApplicationState_Startup::OnEnterState( EApplicationState previousState )
{
    if ( previousState == EApplicationState::Invalid )
    {
        if ( !MasterBuilderApplication::DB )
        {
            MasterBuilderApplication::DB = new CitizenDB;
        }

        if ( !MasterBuilderApplication::Socket )
        {
            MasterBuilderApplication::Socket = new MasterSocket;
        }
    }
}

void ApplicationState_Startup::OnExitState( EApplicationState nextState )
{
}