
#include "World/World.h"
#include "SDL_collide.h"


#include "Parcours/include/ParcoursAgentObserver.h"
#include "Parcours/include/ParcoursSharedData.h"


ParcoursAgentObserver::ParcoursAgentObserver( )
{
}

ParcoursAgentObserver::ParcoursAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (ParcoursAgentWorldModel*)__wm;
}

ParcoursAgentObserver::~ParcoursAgentObserver()
{
	// nothing to do.
}

void ParcoursAgentObserver::reset()
{
	// nothing to do.
}

void ParcoursAgentObserver::step()
{
	RobotAgentPtr agent = gWorld->getAgent(_wm->_agentId);
	// nothing to do.
	if (gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		ParcoursSharedData::locationLogFile << _wm->_agentId << "," << gWorld->getIterations();
		
		int orgSize = 1;
		int orgId = 0;
		if(agent->isPartOfOrganism()){
			orgSize = agent->getOrganism()->size();
			orgId = agent->getOrganism()->getId();
		}
		ParcoursSharedData::locationLogFile << "," << _wm->_xReal << "," << _wm->_yReal << "," << agent->isPartOfOrganism() << "," << orgSize << "," << orgId << "," << _wm->swarmTime << "," << _wm->organismTime << std::endl;
	}
}
