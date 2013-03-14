/*
 *  BattleWorldObserver.h
 *  roborobo-online
 *
 *  Created by Jean-Marc on 01/02/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef BATTLEWORLDOBSERVER_H
#define BATTLEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/Observer.h"

class World;

class BattleWorldObserver : public WorldObserver
{
	protected:
//		World *_world;
		std::map<int, int> delayedEnergyPoints;
		int _energyPointUsed;
		bool _firstStep;
		std::vector< std::vector<double> > _genomesToLoad;
		int _nbGenomeToLoad;
		
	public:
		//Initializes the variables
		BattleWorldObserver( World *__world );
		~BattleWorldObserver();
				
		void reset();
		void step();
		void incEnergyPointUsed();
		void loadGenomes();

};

#endif

