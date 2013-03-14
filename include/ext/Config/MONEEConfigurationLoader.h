/* Simple shells */

#ifndef MONEECONFIGURATIONLOADER_H
#define	MONEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class MONEEConfigurationLoader : public ConfigurationLoader
{
	public:
		MONEEConfigurationLoader();
		~MONEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
