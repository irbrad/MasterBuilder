//
//  ApplicationState_Shutdown.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__ApplicationState_Shutdown__
#define __MasterBuilder__ApplicationState_Shutdown__

#include "ApplicationState.h"

/**
 *  Shutting everything down now, don't fuck up!
 */
class ApplicationState_Shutdown : public IApplicationState
{
public:
    ApplicationState_Shutdown();

    EApplicationState GetState() const override
    {
        return EApplicationState::Shutdown;
    }

    EApplicationState NextState() const override
    {
        return EApplicationState::Shutdown;
    }

    const char* GetStateName() const override { return "Shutdown"; }

    void Update() override;
    void OnEnterState( EApplicationState previousState ) override;
    void OnExitState( EApplicationState nextState ) override;
};

#endif /* defined(__MasterBuilder__ApplicationState_Shutdown__) */
