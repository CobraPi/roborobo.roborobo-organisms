#include "Config/SelfRepConfigurationLoader.h"

#include "SelfReplication/include/SelfRepWorldObserver.h"
#include "SelfReplication/include/SelfRepAgentWorldModel.h"
#include "SelfReplication/include/SelfRepAgentObserver.h"
#include "SelfReplication/include/SelfRepPerceptronControlArchitecture.h"
#include "WorldInterface/RobotAgentWorldInterfaceDangerWorld.h"

SelfRepConfigurationLoader::SelfRepConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

SelfRepConfigurationLoader::~SelfRepConfigurationLoader()
{
	//nothing to do
}


WorldObserver* SelfRepConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SelfRepWorldObserver(wm);
}

RobotAgentWorldModel* SelfRepConfigurationLoader::make_RobotAgentWorldModel()
{
	return new SelfRepAgentWorldModel();
}

AgentObserver* SelfRepConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new SelfRepAgentObserver(wm);
}

BehaviorControlArchitecture* SelfRepConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new SelfRepPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* SelfRepConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}
