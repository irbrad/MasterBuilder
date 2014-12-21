//
//  Application.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__Application__
#define __MasterBuilder__Application__

/**
 *  Our main application, doesn't really do anything but update app states
 */
class Application
{
public:
    Application( class IApplicationState* InitialState );

    /**
     *  This boolean is checked constantly to determine if the app should exit
     *
     *  @return should we shutdown?
     */
    bool Shutdown() const { return ShouldShutdown; }

    /**
     *  Tick as fast as we can!
     */
    virtual void Update();

protected:
    /**
     *  Create a new application state
     *
     *  @param state the new state we're creating
     *
     *  @return a new application state
     */
    virtual class IApplicationState* CreateAppState( unsigned state ) const = 0;

    bool ShouldShutdown;
    class IApplicationState* CurrentState;
};

#endif /* defined(__MasterBuilder__Application__) */
