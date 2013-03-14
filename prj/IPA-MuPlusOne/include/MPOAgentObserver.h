/*
 *  MPOAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef MPOAGENTOBSERVER_H
#define MPOAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "IPA-MuPlusOne/include/MPOAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class MPOAgentObserver : public AgentObserver
{
	protected:
		MPOAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		MPOAgentObserver( );
		MPOAgentObserver( RobotAgentWorldModel *__wm );
		~MPOAgentObserver();
				
		void reset();
		void step();
		
};


#endif

