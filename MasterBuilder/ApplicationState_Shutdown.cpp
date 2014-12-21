//
//  ApplicationState_Shutdown.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "ApplicationState_Shutdown.h"
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
    if ( CitizenDB::instance )
    {
        delete CitizenDB::instance;
        CitizenDB::instance = nullptr;
    }

    if ( MasterSocket::instance )
    {
        delete MasterSocket::instance;
        MasterSocket::instance = nullptr;
    }
}

void ApplicationState_Shutdown::OnExitState( EApplicationState nextState )
{
}