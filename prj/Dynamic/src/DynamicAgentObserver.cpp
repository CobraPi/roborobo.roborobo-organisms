
#include "World/World.h"
#include "SDL_collide.h"


#include "Dynamic/include/DynamicAgentObserver.h"
#include "Dynamic/include/DynamicSharedData.h"


DynamicAgentObserver::DynamicAgentObserver( )
{
}

DynamicAgentObserver::DynamicAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (DynamicAgentWorldModel*)__wm;
}

DynamicAgentObserver::~DynamicAgentObserver()
{
	// nothing to do.
}

void DynamicAgentObserver::reset()
{
	// nothing to do.
}

void DynamicAgentObserver::step()
{
    if(this->_wm->joinedOrganism){
        this->_wm->joinedOrganism =false;
        
        RobotAgentPtr robot = gWorld->getAgent(this->_wm->_agentId);
        Organism *organism = robot->getOrganism();
        redistributeEnergy(organism);
    }
    
    RobotAgentPtr agent = gWorld->getAgent(_wm->_agentId);
	// nothing to do.
	if (gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		DynamicSharedData::locationLogFile << _wm->_agentId << "," << gWorld->getIterations();
		
		int orgSize = 1;
		int orgId = 0;
		if(agent->isPartOfOrganism()){
			orgSize = agent->getOrganism()->size();
			orgId = agent->getOrganism()->getId();
		}
		DynamicSharedData::locationLogFile << "," << _wm->_xReal << "," << _wm->_yReal << "," << agent->isPartOfOrganism() << "," << orgSize << "," << orgId << "," << _wm->swarmTime << "," << _wm->organismTime << std::endl;
	}
}

/**
 * Redistribute energy over the organism
 */
void DynamicAgentObserver::redistributeEnergy(Organism *organism){
    std::vector<RobotAgentPtr>::iterator it;
    
    double totalEnergy = 0;
    
    for (it = organism->agents.begin(); it != organism->agents.end(); it++) {
		RobotAgentWorldModel* worldModel = (*it)->getWorldModel();
        
        totalEnergy += worldModel->getEnergyLevel();
    }
    
    double energyPerAgent = totalEnergy / organism->size();
    for (it = organism->agents.begin(); it != organism->agents.end(); it++) {
		RobotAgentWorldModel* worldModel = (*it)->getWorldModel();
        
        double deltaEnergy = worldModel->getEnergyLevel() - energyPerAgent;
        
        worldModel->setDeltaEnergy(deltaEnergy);
        worldModel->setEnergyLevel(energyPerAgent);
    }
}