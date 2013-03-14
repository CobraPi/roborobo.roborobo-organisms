/*
 *  MPOWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "IPA-HiveMind/include/HiveMindWorldObserver.h"

#include "World/World.h"


HiveMindWorldObserver::HiveMindWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

HiveMindWorldObserver::~HiveMindWorldObserver()
{
	// nothing to do.
}

void HiveMindWorldObserver::reset()
{
	// nothing to do.
}

void HiveMindWorldObserver::step()
{
	// nothing to do.
}
