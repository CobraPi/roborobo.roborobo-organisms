/*
 *  BasicAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef MONEEAGENTOBSERVER_H
#define MONEEAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

class MONEEAgentWorldModel;


class MONEEAgentObserver : public AgentObserver
{
	protected:
	MONEEAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		MONEEAgentObserver( );
		MONEEAgentObserver( RobotAgentWorldModel *__wm );
		~MONEEAgentObserver();
				
		void reset();
		void step();
		
};


#endif

