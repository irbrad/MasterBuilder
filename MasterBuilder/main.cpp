//
//  main.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "MasterBuilderApplication.h"

#include <google/protobuf/message.h>

int main( int argc, const char* argv[] )
{
    // insert code here...
    MasterBuilderApplication application;

    while ( !application.Shutdown() )
    {
        application.Update();
    }

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
