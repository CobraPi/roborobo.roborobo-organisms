/*
 *  MPOWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "IPA-MuPlusOne/include/MPOWorldObserver.h"

#include "World/World.h"


MPOWorldObserver::MPOWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

MPOWorldObserver::~MPOWorldObserver()
{
	// nothing to do.
}

void MPOWorldObserver::reset()
{
	// nothing to do.
}

void MPOWorldObserver::step()
{
	// nothing to do.
}


