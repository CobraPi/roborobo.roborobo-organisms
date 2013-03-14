/*
 *  MedeaTwoSunsAgentObserver.h
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

#include "mEDEA-twosuns/include/MedeaTwoSunsAgentWorldModel.h"

#include "mEDEA-twosuns/include/MedeaTwoSunsSharedData.h"
 
class MedeaTwoSunsAgentObserver : public AgentObserver
{
	protected:
		MedeaTwoSunsAgentWorldModel	*_wm;

		void pickRandomGenome();
		void mutateWithBouncingBounds(float sigma);

	public:
		MedeaTwoSunsAgentObserver( RobotAgentWorldModel *__wm );
		~MedeaTwoSunsAgentObserver();
				
		void reset();
		void step();

		void writeGenome(std::vector<double> genome, int senderId, float sigma);

};

#endif
