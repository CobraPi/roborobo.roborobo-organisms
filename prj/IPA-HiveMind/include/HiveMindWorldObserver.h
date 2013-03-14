/*
 *  HiveMindWorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */



#ifndef HIVEMINDWORLDOBSERVER_H
#define HIVEMINDWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

class World;

class HiveMindWorldObserver : public WorldObserver
{
	protected:
		
	public:
		HiveMindWorldObserver( World *__world );
		~HiveMindWorldObserver();
				
		void reset();
		void step();
		
};

#endif

