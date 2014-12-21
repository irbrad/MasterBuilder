//
//  ApplicationState_Running.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__ApplicationState_Running__
#define __MasterBuilder__ApplicationState_Running__

#include "ApplicationState.h"

/**
 *  The application is running now
 *
 *  In this state we're observing our socket and processing information coming
 *  from each of the slaves (citizens) and responding to their requests.
 */
class ApplicationState_Running : public IApplicationState
{
public:
    ApplicationState_Running();

    EApplicationState GetState() const override
    {
        return EApplicationState::Running;
    }

    EApplicationState NextState() const override;
    const char* GetStateName() const override { return "Running"; }

    void Update() override;
    void OnEnterState( EApplicationState previousState ) override;
    void OnExitState( EApplicationState nextState ) override;

private:
};

#endif /* defined(__MasterBuilder__ApplicationState_Running__) */
