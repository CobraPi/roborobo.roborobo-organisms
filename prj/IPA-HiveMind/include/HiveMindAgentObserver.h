/*
 *  MPOAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef HIVEMINDAGENTOBSERVER_H
#define HIVEMINDAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "IPA-HiveMind/include/HiveMindAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class HiveMindAgentObserver : public AgentObserver
{
	protected:
		HiveMindAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		HiveMindAgentObserver( );
		HiveMindAgentObserver( RobotAgentWorldModel *__wm );
		~HiveMindAgentObserver();
				
		void reset();
		void step();
		
};


#endif

