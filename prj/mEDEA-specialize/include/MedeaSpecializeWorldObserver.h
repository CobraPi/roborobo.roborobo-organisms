/*
 *  MedeaSpecializeWorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 15/09/10
 *
 */



#ifndef MEDEASPECIALIZEWORLDOBSERVER_H
#define MEDEASPECIALIZEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/Observer.h"

#include "mEDEA-specialize/include/MedeaSpecializeSharedData.h"
#include "mEDEA-specialize/include/MedeaSpecializeAgentWorldModel.h"

#include "mEDEA-specialize/include/MedeaSpecializeSharedData.h"

class World;

class MedeaSpecializeWorldObserver : public WorldObserver
{
	private:
		void updateExperimentalSettings(); // check and update (only if required) experimental setting.
		void updateEnvironmentResources();
		void updateAllAgentsEnergyLevel();

	protected:
		int _generationCount;
		int _lifeIterationCount;

	public:
		MedeaSpecializeWorldObserver( World *__world );
		~MedeaSpecializeWorldObserver();
				
		void reset();
		void step();
		
		int getLifeIterationCount();

};

#endif