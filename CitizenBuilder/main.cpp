//
//  main.cpp
//  CitizenBuilder
//
//  Created by Sammy James on 12/20/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "CitizenBuilder.h"

int main( int argc, const char* argv[] )
{
    CitizenBuilder application;

    while ( !application.Shutdown() )
    {
        application.Update();
    }

    return 0;
}
