//
//  MasterBuilderApplication.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__MasterBuilderApplication__
#define __MasterBuilder__MasterBuilderApplication__

#include "Application.h"

/**
 *  Our main application, doesn't really do anything but update app states
 */
class MasterBuilderApplication : public Application
{
public:
    MasterBuilderApplication();

protected:
    class IApplicationState* CreateAppState( unsigned state ) const override;
};

#endif /* defined(__MasterBuilder__MasterBuilderApplication__) */
