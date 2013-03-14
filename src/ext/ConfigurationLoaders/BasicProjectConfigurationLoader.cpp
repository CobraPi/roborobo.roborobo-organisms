#include "Config/BasicProjectConfigurationLoader.h"

#include "BasicProject/include/BasicWorldObserver.h"
#include "BasicProject/include/BasicAgentWorldModel.h"
#include "BasicProject/include/BasicAgentObserver.h"
#include "BasicProject/include/BasicControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

BasicProjectConfigurationLoader::BasicProjectConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

BasicProjectConfigurationLoader::~BasicProjectConfigurationLoader()
{
	//nothing to do
}


WorldObserver* BasicProjectConfigurationLoader::make_WorldObserver(World* wm)
{
	return new BasicWorldObserver(wm);
}

RobotAgentWorldModel* BasicProjectConfigurationLoader::make_RobotAgentWorldModel()
{
	return new BasicAgentWorldModel();
}

AgentObserver* BasicProjectConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new BasicAgentObserver(wm);
}

BehaviorControlArchitecture* BasicProjectConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new BasicControlArchitecture(wm);
}

RobotAgentWorldInterface* BasicProjectConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}
