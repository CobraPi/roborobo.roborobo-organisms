#include "Config/MONEEConfigurationLoader.h"

#include "MONEE/include/MONEEWorldObserver.h"
#include "MONEE/include/MONEEAgentWorldModel.h"
#include "MONEE/include/MONEEAgentObserver.h"
#include "MONEE/include/MONEEControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

MONEEConfigurationLoader::MONEEConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

MONEEConfigurationLoader::~MONEEConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MONEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MONEEWorldObserver(wm);
}

RobotAgentWorldModel* MONEEConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MONEEAgentWorldModel();
}

AgentObserver* MONEEConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MONEEAgentObserver(wm);
}

BehaviorControlArchitecture* MONEEConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MONEEControlArchitecture(wm);
}

RobotAgentWorldInterface* MONEEConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}
