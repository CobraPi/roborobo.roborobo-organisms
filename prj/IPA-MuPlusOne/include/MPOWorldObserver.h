/*
 *  MPOWorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */



#ifndef MPOWORLDOBSERVER_H
#define MPOWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

class World;

class MPOWorldObserver : public WorldObserver
{
	protected:
		
	public:
		MPOWorldObserver( World *__world );
		~MPOWorldObserver();
				
		void reset();
		void step();
		
};

#endif

