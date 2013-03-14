/*
 *  MedeaSpecializeAgentObserver.h
 *  Roborobo
 *
 *  created by Nicolas on 15/09/10
 *
 */

#ifndef MEDEASPECIALIZEAGENTOBSERVER_H
#define MEDEASPECIALIZEAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "mEDEA-specialize/include/MedeaSpecializeAgentWorldModel.h"

#include "mEDEA-specialize/include/MedeaSpecializeSharedData.h"
 
class MedeaSpecializeAgentObserver : public AgentObserver
{
	protected:
		MedeaSpecializeAgentWorldModel	*_wm;

		void pickRandomGenome();
		void mutateWithBouncingBounds(float sigma);

	public:
		MedeaSpecializeAgentObserver( RobotAgentWorldModel *__wm );
		~MedeaSpecializeAgentObserver();
				
		void reset();
		void step();

		void writeGenome(std::vector<double> genome, int senderId, float sigma);

};

#endif
