/*
 *  SelfRepWorldObserver.h
 *  roborobo-online
 *
 *  Created by Jean-Marc on 01/02/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef SELFREPWORLDOBSERVER_H
#define SELFREPWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/Observer.h"

class World;

class SelfRepWorldObserver : public WorldObserver
{
	private:
//		World *_world;
		double _sigmaRef;
		double _iterationMax;
		
	public:
		//Initializes the variables
		SelfRepWorldObserver( World *__world );
		~SelfRepWorldObserver();
				
		void reset();
		void step();
		
};

#endif

