//
//  CitizenBuilder.h
//  MasterBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__CitizenBuilder__
#define __MasterBuilder__CitizenBuilder__

#include <stdio.h>

#include "Application.h"

class CitizenBuilder : public Application
{
public:
    CitizenBuilder();

    void Update() override;

protected:
    class IApplicationState* CreateAppState( unsigned state ) const override;
};

#endif /* defined(__MasterBuilder__CitizenBuilder__) */
