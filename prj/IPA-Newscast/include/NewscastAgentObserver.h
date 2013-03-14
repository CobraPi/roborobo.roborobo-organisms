/*
 *  MPOAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef NEWSCASTAGENTOBSERVER_H
#define NEWSCASTAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "IPA-Newscast/include/NewscastAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class NewscastAgentObserver : public AgentObserver
{
	protected:
		NewscastAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		NewscastAgentObserver( );
		NewscastAgentObserver( RobotAgentWorldModel *__wm );
		~NewscastAgentObserver();
				
		void reset();
		void step();
		
};


#endif

