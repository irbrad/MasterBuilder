//
//  ApplicationState.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef MasterBuilder_ApplicationState_h
#define MasterBuilder_ApplicationState_h

#include <stdint.h>

enum EApplicationState
{
    Invalid,  // invalid state
    Startup,  // application is starting, allocate our resources
    Running,  // application is running, we're processing message
    Shutdown, // we're shutting down, clean everything up
};

class IApplicationState
{
public:
    /**
     *  Get the current state of the application
     *
     *  @return EApplicationState
     */
    virtual EApplicationState GetState() const = 0;

    /**
     *  The next state we should transition to
     *
     *  @return EApplicationState
     */
    virtual EApplicationState NextState() const = 0;

    /**
     *  Get a human readable state name
     *
     *  @return a string
     */
    virtual const char* GetStateName() const = 0;

    /**
     *  Update the application state, functionally your tick
     */
    virtual void Update() = 0;

    /**
     *  Called when transitioning from one state to another
     *
     *  @param previousState the state from which we came
     */
    virtual void OnEnterState( EApplicationState previousState ) = 0;

    /**
     *  Called when transitioning from one state to another
     *
     *  @param nextState the state into which we're transitioning
     */
    virtual void OnExitState( EApplicationState nextState ) = 0;
};

#endif
