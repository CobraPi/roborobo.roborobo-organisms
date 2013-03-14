/*
 *  WorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 25/05/10
 *
 */



#ifndef MEDEAWORLDOBSERVER_H
#define MEDEAWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/Observer.h"

#include "mEDEA/include/MedeaSharedData.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"

class World;

class MedeaWorldObserver : public WorldObserver
{
	private:
		void updateExperimentalSettings(); // check and update (only if required) experimental setting.
		void updateEnvironmentResources();
		void updateAllAgentsEnergyLevel();

	protected:
		int _generationCount;
		int _lifeIterationCount;

	public:
		MedeaWorldObserver( World *__world );
		~MedeaWorldObserver();
				
		void reset();
		void step();
		
		int getLifeIterationCount();

};

#endif