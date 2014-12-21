//
//  CitizenBuilder.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "CitizenBuilder.h"

#include "ApplicationState.h"
#include "AppState_Startup.h"
#include "AppState_Running.h"
#include "AppState_Shutdown.h"

CitizenBuilder::CitizenBuilder()
    : Application( CreateAppState( EApplicationState::Startup ) )
{
}

IApplicationState* CitizenBuilder::CreateAppState( unsigned state ) const
{
    switch ( state )
    {
    case EApplicationState::Startup: return new AppState_Startup;
    case EApplicationState::Running: return new AppState_Running;
    case EApplicationState::Shutdown: return new AppState_Shutdown;
    case EApplicationState::Invalid: // intentional fallthrough
    default: return nullptr;
    }
}