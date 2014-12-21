//
//  SlaveDB.h
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#ifndef __MasterBuilder__SlaveDB__
#define __MasterBuilder__SlaveDB__

#include <stdint.h>
#include <map>
#include <string>
#include <bitset>

class CitizenDB
{
public:
    /**
     *  Register a citizen to the DB
     *
     *  @param Host       the ip address of the slave
     *  @param CoreCount  supported hardware parallelism
     *  @param PowerIndex precalculated power rating
     */
    void Register( const std::string& Host, uint8_t CoreCount, uint8_t PowerIndex );

    /**
     *  Release a host
     *
     *  @param Host the host we should release
     */
    void Release( const std::string& Host );

    /**
     *  Query the DB for information about the current state of a citizen
     *
     *  @param Host       the ip address of the slave
     *  @param CoreCount  ref this slaves supported parallel tasks
     *  @param PowerIndex ref this slaves precalculated power rating
     *  @param CoreUsage  ref this slaves current core usage
     *
     *  @return weather or not we found a slave at the provided ip
     */
    bool Query( const std::string& Host, uint8_t& CoreCount, uint8_t& PowerIndex,
                uint32_t& CoreUsage );

    /**
     *  Request a citizen's core for building
     *
     *  @param Host the destination system
     *  @param Core the destination core
     *
     *  @return weather or not we found a host to give out
     */
    bool RequestCore( std::string& Host, uint8_t& Core );

    /**
     *  Return a citizen's core back to the master
     *
     *  @param Host the host from which we're releasing a core
     *  @param Core the core we've released
     */
    void ReleaseCore( const std::string& Host, uint8_t Core );

private:
    struct Citizen
    {
        std::string HostName;
        uint8_t CoreCount;
        uint32_t CoreUsage;
    };

    std::multimap< uint8_t, Citizen, std::greater< uint8_t > > Citizens;
};

#endif /* defined(__MasterBuilder__SlaveDB__) */
