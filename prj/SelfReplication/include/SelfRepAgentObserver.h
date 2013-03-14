/*
 *  SelfRepAgentObserver.h
 *  Roborobo
 *
 *  Created by Jean-Marc on 04/03/10
 *
 */

#ifndef SELFREPAGENTOBSERVER_H
#define SELFREPAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "WorldModels/RobotAgentWorldModel.h"
#include "SelfReplication/include/SelfRepAgentWorldModel.h"

#include "Utilities/LogManager.h"


class SelfRepAgentObserver : public AgentObserver
{
	protected:
		RobotAgentWorldModel	*_wm;
		LogManager *__logManager;

	public:
		SelfRepAgentObserver( RobotAgentWorldModel *__wm );
		~SelfRepAgentObserver();
				
		void step();
		void reset();
};

#endif
