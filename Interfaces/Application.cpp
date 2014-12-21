//
//  Application.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "Application.h"
#include "ApplicationState.h"

Application::Application( IApplicationState* InitialState )
    : ShouldShutdown( false )
    , CurrentState( InitialState )
{
    CurrentState->OnEnterState( EApplicationState::Invalid );
}

void Application::Update()
{
    EApplicationState nextState = EApplicationState::Invalid;
    EApplicationState currentState = EApplicationState::Invalid;
    if ( CurrentState )
    {
        CurrentState->Update();
        currentState = CurrentState->GetState();
        nextState = CurrentState->NextState();
    }

    if ( nextState != currentState )
    {
        IApplicationState* previousState = CurrentState;
        CurrentState = CreateAppState( nextState );

        previousState->OnExitState( CurrentState->GetState() );

        if ( CurrentState )
        {
            CurrentState->OnEnterState( previousState->GetState() );
        }

        delete previousState;
        previousState = nullptr;
    }

    if ( !CurrentState )
    {
        ShouldShutdown = true;
    }
}