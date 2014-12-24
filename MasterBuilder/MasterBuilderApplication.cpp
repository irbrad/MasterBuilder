//
//  MasterBuilderApplication.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include <unistd.h>	// usleep()
#include "MasterBuilderApplication.h"

#include "ApplicationState.h"

#include "ApplicationState_Startup.h"
#include "ApplicationState_Running.h"
#include "ApplicationState_Shutdown.h"

#include "CitizenDB.h"
#include "MasterSocket.h"

/*static*/ CitizenDB* MasterBuilderApplication::DB = nullptr;
/*static*/ MasterSocket* MasterBuilderApplication::Socket = nullptr;

MasterBuilderApplication::MasterBuilderApplication()
    : Application( CreateAppState( EApplicationState::Startup ) )
{
}

IApplicationState* MasterBuilderApplication::CreateAppState( unsigned state ) const
{
    switch ( state )
    {
        case EApplicationState::Startup: return new ApplicationState_Startup;
        case EApplicationState::Running: return new ApplicationState_Running;
        case EApplicationState::Shutdown: return new ApplicationState_Shutdown;
        case EApplicationState::Invalid: // intentional fallthrough
        default: return nullptr;
    }
}

void MasterBuilderApplication::Update()
{
    Application::Update();

    usleep( 1000000 );
}
