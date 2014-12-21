//
//  SlaveDB.cpp
//  MasterBuilder
//
//  Created by Sammy James on 12/19/14.
//  Copyright (c) 2014 Pawkette. All rights reserved.
//

#include "CitizenDB.h"

/*static*/ CitizenDB* CitizenDB::instance = nullptr;

void CitizenDB::Register( const std::string& Host, uint8_t CoreCount,
                          uint8_t PowerIndex )
{
    const auto& range = Citizens.equal_range( PowerIndex );
    if ( range.first == range.second )
    {
        Citizens.insert(
            std::make_pair( PowerIndex, Citizen{Host, CoreCount, 0} ) );
    }
    else
    {
        bool located = false;
        for ( auto it = range.first; it != range.second; ++it )
        {
            if ( it->second.HostName == Host )
            {
                // already have this host.
                if ( it->second.CoreCount != CoreCount )
                {
                    it->second.CoreCount = CoreCount;
                }

                located = true;
                break;
            }
        }

        if ( !located )
        {
            Citizens.insert(
                std::make_pair( PowerIndex, Citizen{Host, CoreCount, 0} ) );
        }
    }
}

void CitizenDB::Release( const std::string& Host )
{
    for ( auto it = Citizens.begin(); it != Citizens.end(); ++it )
    {
        if ( it->second.HostName == Host )
        {
            Citizens.erase( it, ++it );
            return;
        }
    }
}

bool CitizenDB::Query( const std::string& Host, uint8_t& CoreCount,
                       uint8_t& PowerIndex, uint32_t& CoreUsage )
{
    for ( const auto it : Citizens )
    {
        if ( it.second.HostName == Host )
        {
            PowerIndex = it.first;
            CoreCount = it.second.CoreCount;
            CoreUsage = it.second.CoreUsage;
            return true;
        }
    }
    return false;
}

bool CitizenDB::RequestCore( std::string& Host, uint8_t& Core )
{
    for ( auto& it : Citizens )
    {
        for ( uint8_t i = 0; i < it.second.CoreCount; ++i )
        {
            if ( it.second.CoreUsage >> i == 0 )
            {
                Core = i;
                Host = it.second.HostName;
                it.second.CoreUsage |= ( 1 << i );
                break;
            }
        }

        if ( !Host.empty() )
        {
            break;
        }
    }

    return !Host.empty();
}

void CitizenDB::ReleaseCore( const std::string& Host, uint8_t Core )
{
    for ( auto& it : Citizens )
    {
        if ( it.second.HostName == Host )
        {
            it.second.CoreUsage &= ~( 1 << Core );
            break;
        }
    }
}
