/*
 *  NewscastWorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */



#ifndef NEWSCASTWORLDOBSERVER_H
#define NEWSCASTWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

class World;

class NewscastWorldObserver : public WorldObserver
{
	protected:
		
	public:
		NewscastWorldObserver( World *__world );
		~NewscastWorldObserver();
				
		void reset();
		void step();
		
};

#endif

