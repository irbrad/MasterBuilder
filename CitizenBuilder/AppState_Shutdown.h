//
//  AppState_Shutdown.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__AppState_Shutdown__
#define __MasterBuilder__AppState_Shutdown__

#include "ApplicationState.h"

class AppState_Shutdown : public IApplicationState
{
public:
    EApplicationState GetState() const override
    {
        return EApplicationState::Shutdown;
    }

    EApplicationState NextState() const override { return NextAppState; }

    const char* GetStateName() const override { return "Shutdown"; }

    void Update() override;
    void OnEnterState( EApplicationState previousState ) override;
    void OnExitState( EApplicationState nextState ) override;

private:
    EApplicationState NextAppState;
};

#endif /* defined(__MasterBuilder__AppState_Shutdown__) */
