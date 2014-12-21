//
//  ApplicationState_Startup.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__ApplicationState_Startup__
#define __MasterBuilder__ApplicationState_Startup__

#include "ApplicationState.h"

/**
 *  The state of the application during startup.
 *
 *  We do most of our initial allocation during this state, including the
 *  master socket, and slave db.
 */
class ApplicationState_Startup : public IApplicationState
{
public:
    ApplicationState_Startup();

    EApplicationState GetState() const override
    {
        return EApplicationState::Startup;
    }

    EApplicationState NextState() const override { return NextAppState; }
    const char* GetStateName() const override { return "Startup"; }

    void Update() override;
    void OnEnterState( EApplicationState previousState ) override;
    void OnExitState( EApplicationState nextState ) override;

private:
    EApplicationState NextAppState;
};

#endif /* defined(__MasterBuilder__ApplicationState_Startup__) */
