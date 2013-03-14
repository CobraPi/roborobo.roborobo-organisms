
#include "World/World.h"
#include "SDL_collide.h"


#include "ChangeDetection/include/ChangeDetectionAgentObserver.h"
#include "ChangeDetection/include/ChangeDetectionSharedData.h"


ChangeDetectionAgentObserver::ChangeDetectionAgentObserver( )
{
}

ChangeDetectionAgentObserver::ChangeDetectionAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (ChangeDetectionAgentWorldModel*)__wm;
}

ChangeDetectionAgentObserver::~ChangeDetectionAgentObserver()
{
	// nothing to do.
}

void ChangeDetectionAgentObserver::reset()
{
	// nothing to do.
}

void ChangeDetectionAgentObserver::step()
{
	RobotAgentPtr agent = gWorld->getAgent(_wm->_agentId);
	// nothing to do.
	if (gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		ChangeDetectionSharedData::locationLogFile << _wm->_agentId << "," << gWorld->getIterations();
		
		int orgSize = 1;
		int orgId = 0;
		if(agent->isPartOfOrganism()){
			orgSize = agent->getOrganism()->size();
			orgId = agent->getOrganism()->getId();
		}
		ChangeDetectionSharedData::locationLogFile << "," << _wm->_xReal << "," << _wm->_yReal << "," << agent->isPartOfOrganism() << "," << orgSize << "," << orgId << "," << _wm->swarmTime << "," << _wm->organismTime << std::endl;
	}
}
