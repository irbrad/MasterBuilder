//
//  ApplicationState_Shutdown.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "ApplicationState_Shutdown.h"

#include "MasterBuilderApplication.h"
#include "CitizenDB.h"
#include "MasterSocket.h"

ApplicationState_Shutdown::ApplicationState_Shutdown()
{
}

void ApplicationState_Shutdown::Update()
{
}

void ApplicationState_Shutdown::OnEnterState( EApplicationState previousState )
{
    if ( MasterBuilderApplication::DB )
    {
        delete MasterBuilderApplication::DB;
        MasterBuilderApplication::DB = nullptr;
    }

    if ( MasterBuilderApplication::Socket )
    {
        delete MasterBuilderApplication::Socket;
        MasterBuilderApplication::Socket = nullptr;
    }
}

void ApplicationState_Shutdown::OnExitState( EApplicationState nextState )
{
}