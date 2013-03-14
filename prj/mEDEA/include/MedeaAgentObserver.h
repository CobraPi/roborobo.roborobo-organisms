/*
 *  MedeaAgentObserver.h
 *  Roborobo
 *
 *  imported from Jean-Marc on 15/12/09
 *  current dev: Nicolas on 1/4/2009
 *
 */

#ifndef MEDEAAGENTOBSERVER_H
#define MEDEAAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "mEDEA/include/MedeaAgentWorldModel.h"

#include "mEDEA/include/MedeaSharedData.h"
 
class MedeaAgentObserver : public AgentObserver
{
	protected:
		MedeaAgentWorldModel	*_wm;

		void pickRandomGenome();
		void mutateWithBouncingBounds(float sigma);

	public:
		MedeaAgentObserver( RobotAgentWorldModel *__wm );
		~MedeaAgentObserver();
				
		void reset();
		void step();

		void writeGenome(std::vector<double> genome, int senderId, float sigma);

};

#endif
