/*
 *  ReplayWorldObserver.h
 *  roborobo-online
 *
 *  Created by Jean-Marc on 20/04/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef REPLAYWORLDOBSERVER_H
#define REPLAYWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/Observer.h"

class World;

class ReplayWorldObserver : public WorldObserver
{
	private:
//		World *_world;
		bool _firstStep;
		std::vector<double> _genomeToReplay;
		
	public:
		//Initializes the variables
		ReplayWorldObserver( World *__world );
		~ReplayWorldObserver();
				
		void reset();
		void step();
		void loadGenome();

};

#endif

