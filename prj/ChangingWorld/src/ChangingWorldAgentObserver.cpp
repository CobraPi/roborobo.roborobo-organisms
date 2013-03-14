
#include "World/World.h"
#include "SDL_collide.h"


#include "ChangingWorld/include/ChangingWorldAgentObserver.h"
#include "ChangingWorld/include/ChangingWorldSharedData.h"


ChangingWorldAgentObserver::ChangingWorldAgentObserver( )
{
}

ChangingWorldAgentObserver::ChangingWorldAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (ChangingWorldAgentWorldModel*)__wm;
}

ChangingWorldAgentObserver::~ChangingWorldAgentObserver()
{
	// nothing to do.
}

void ChangingWorldAgentObserver::reset()
{
	// nothing to do.
}

void ChangingWorldAgentObserver::step()
{
	RobotAgentPtr agent = gWorld->getAgent(_wm->_agentId);
	// nothing to do.
	if (gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		ChangingWorldSharedData::locationLogFile << _wm->_agentId << "," << gWorld->getIterations();
		
		int orgSize = 1;
		int orgId = 0;
		if(agent->isPartOfOrganism()){
			orgSize = agent->getOrganism()->size();
			orgId = agent->getOrganism()->getId();
		}
		ChangingWorldSharedData::locationLogFile << "," << _wm->_xReal << "," << _wm->_yReal << "," << agent->isPartOfOrganism() << "," << orgSize << "," << orgId << std::endl;
	}
}
