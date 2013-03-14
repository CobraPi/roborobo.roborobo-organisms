/*
 *  ReplayAgentObserver.h
 *  Roborobo
 *
 *  Created by Jean-Marc on 20/04/10
 *
 */

#ifndef REPLAY_AGENT_OBSERVER_H
#define REPLAY_AGENT_OBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "mEDEA/include/MedeaAgentWorldModel.h"

#include "mEDEA/include/MedeaSharedData.h"
#include "Utilities/LogManager.h"

class ReplayAgentObserver : public AgentObserver
{
	protected:
		MedeaAgentWorldModel	*_wm;

		std::vector<double> _currentGenome;

		int _nbGene;
		int _nbLayer;
		int _nbHiddenNeurons;

		int _iterationCount ;

		void logStatus();
		LogManager *__logManager;

	public:
		ReplayAgentObserver( RobotAgentWorldModel *__wm );
		~ReplayAgentObserver();
		void loadGenome(std::vector<double> inGenome);
				
		void reset();
		void step();
};

#endif
