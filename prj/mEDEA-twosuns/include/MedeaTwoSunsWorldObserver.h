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

#include "mEDEA-twosuns/include/MedeaTwoSunsSharedData.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsAgentWorldModel.h"

#include "mEDEA-twosuns/include/MedeaTwoSunsSharedData.h"

class World;

class MedeaTwoSunsWorldObserver : public WorldObserver
{
	private:
		void updateExperimentalSettings(); // check and update (only if required) experimental setting.
		void updateEnvironmentResources();
		void updateAllAgentsEnergyLevel();

	protected:
		int _generationCount;
		int _lifeIterationCount;

	public:
		MedeaTwoSunsWorldObserver( World *__world );
		~MedeaTwoSunsWorldObserver();
				
		void reset();
		void step();
		
		int getLifeIterationCount();

};

#endif