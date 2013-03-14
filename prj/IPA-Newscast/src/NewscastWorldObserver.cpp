/*
 *  MPOWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "IPA-Newscast/include/NewscastWorldObserver.h"

#include "World/World.h"


NewscastWorldObserver::NewscastWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

NewscastWorldObserver::~NewscastWorldObserver()
{
	// nothing to do.
}

void NewscastWorldObserver::reset()
{
	// nothing to do.
}

void NewscastWorldObserver::step()
{
	// nothing to do.
}
