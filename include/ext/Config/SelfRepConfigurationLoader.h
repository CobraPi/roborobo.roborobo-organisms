/*
 * SelfRepConfigurationLoader.h
 *
 * Created by jm on 20/12/10
 *
 */

#ifndef SELFREPCONFIGURATIONLOADER_H
#define SELFREPCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class SelfRepConfigurationLoader : public ConfigurationLoader
{
	public:
		SelfRepConfigurationLoader();
		~SelfRepConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
