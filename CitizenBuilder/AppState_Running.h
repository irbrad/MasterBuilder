//
//  AppState_Running.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__AppState_Running__
#define __MasterBuilder__AppState_Running__

#include "ApplicationState.h"

class AppState_Running : public IApplicationState
{
public:
    AppState_Running()
        : NextAppState( EApplicationState::Running )
    {
    }

    EApplicationState GetState() const override
    {
        return EApplicationState::Running;
    }

    EApplicationState NextState() const override { return NextAppState; }
    const char* GetStateName() const override { return "Running"; }

    void Update() override;
    void OnEnterState( EApplicationState previousState ) override;
    void OnExitState( EApplicationState nextState ) override;

protected:
    static void SignalHandler( int32_t id );

    EApplicationState NextAppState;
};

#endif /* defined(__MasterBuilder__AppState_Running__) */
