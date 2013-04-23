
#include <math.h>

#include "ChangingWorld/include/ChangingWorldWorldObserver.h"
#include "ChangingWorld/include/ChangingWorldSharedData.h"
#include "ChangingWorld/include/ChangingWorldAgentWorldModel.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "SDL_collide.h"
#include "World/EnergyPoint.h"

ChangingWorldWorldObserver::ChangingWorldWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

ChangingWorldWorldObserver::~ChangingWorldWorldObserver() {
	// nothing to do.
}

void ChangingWorldWorldObserver::showOrganisms() {
}

void ChangingWorldWorldObserver::reset() {
	if(ChangingWorldSharedData::outputLogFile){
		ChangingWorldSharedData::outputLogFile.close();
	}
	ChangingWorldSharedData::outputLogFile.open(ChangingWorldSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ChangingWorldSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << ChangingWorldSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ChangingWorldSharedData::outputLogFile << "id,timestep,outputs,choice,desired organism size,direction,speed" << std::endl;
	
	
	if(ChangingWorldSharedData::locationLogFile){
		ChangingWorldSharedData::locationLogFile.close();
	}
	ChangingWorldSharedData::locationLogFile.open(ChangingWorldSharedData::gLocationLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ChangingWorldSharedData::locationLogFile) {
		std::cout << "[error] Cannot open log file" << ChangingWorldSharedData::gLocationLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ChangingWorldSharedData::locationLogFile << "id,timestep,x,y,in org,org_size,swarm time,organism time" << std::endl;
    
    ChangingWorldSharedData::currentFactor = -1;
}

void ChangingWorldWorldObserver::step() {
    updateAllAgentsEnergy();
    
    int totalStepsPerGeneration = EvolutionaryControlArchitecture::stepsPerCandidate + EvolutionaryControlArchitecture::stepsPerFree + EvolutionaryControlArchitecture::stepsPerRandomFree;
    if (gWorld->getIterations() % (int(totalStepsPerGeneration * ChangingWorldSharedData::factorLifetime)) == 0) {
        ChangingWorldSharedData::currentFactor++;
        if(ChangingWorldSharedData::currentFactor >= ChangingWorldSharedData::scaleFactor.size()){
            ChangingWorldSharedData::currentFactor = 0;
        }
        
        std::cerr << "Iteration: " << gWorld->getIterations() << " scaling factor is now: " << ChangingWorldSharedData::scaleFactor[ChangingWorldSharedData::currentFactor] << std::endl;
    }
}

void ChangingWorldWorldObserver::updateAllAgentsEnergy() {
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		ChangingWorldAgentWorldModel *currentAgentWorldModel = dynamic_cast<ChangingWorldAgentWorldModel*>(_world->getAgent(i)->getWorldModel());
        
		currentAgentWorldModel->setEnergyGained(0);
        
		// * check energy level. Becomes inactive if zero.
		if (currentAgentWorldModel->getEnergyLevel() <= 0) {
			currentAgentWorldModel->setEnergyLevel(0);
			currentAgentWorldModel->setActive(false);
		}
		
		if (currentAgentWorldModel->getEnergyLevel() > 0.0) {
			currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() - 1);
		} else {
			currentAgentWorldModel->setEnergyLevel(0);
		}
        
		if (currentAgentWorldModel->isActive()) {
			Point2d posRobot = currentAgentWorldModel->getPosition();
            ResourceFactory<EnergyPoint>::ResourceFactoryPtr factory = ResourceFactory<EnergyPoint>::getInstance();
            std::vector<ResourceFactory<EnergyPoint>::ResourcePtr> energyPoints = factory->getResources();
//			for (std::vector<EnergyPoint>::iterator it = energyPoints.begin(); it != energyPoints.end(); it++) {
			for(unsigned int j =0; j < energyPoints.size();j++){
                int x1 = posRobot.x;
                int y1 = posRobot.y;
                int r1 = gAgentWidth/2;
                int x2 = energyPoints[j]->getPosition().x;
                int y2 = energyPoints[j]->getPosition().y;
                int r2 = gEnergyPointRadius;
                if (energyPoints[j]->getActiveStatus() && SDL_CollideBoundingCircle(x1,y1,r1,x2,y2,r2,0)) {
					// update energy level
					//std::cout << "Energy Point: " << j << " is giving energy to agent: " << currentAgentWorldModel->_agentId << std::endl;
					
					if (gWorld->getAgent(i)->isPartOfOrganism()) {
						Organism* organism = gWorld->getAgent(i)->getOrganism();
						
						double loadingEnergy = getChangingWorldSupply(organism, j);
                        
						std::vector<RobotAgentPtr>::iterator it2;
						for (it2 = organism->agents.begin(); it2 != organism->agents.end(); it2++) {
//                            std::cout << "Robot " << i << " is harvesting energy point while in an organism " << j << " he will get " << loadingEnergy << " energy, and went from " << currentAgentWorldModel->getEnergyLevel() << " to ";
							ChangingWorldAgentWorldModel* wm = dynamic_cast<ChangingWorldAgentWorldModel*> ((*it2)->getWorldModel());
							if (wm->getEnergyLevel() + loadingEnergy > ChangingWorldSharedData::maxEnergy) {
								wm->setEnergyGained(wm->getEnergyGained() + (ChangingWorldSharedData::maxEnergy - wm->getEnergyLevel()));
								wm->setEnergyLevel(ChangingWorldSharedData::maxEnergy);
							} else {
								wm->setEnergyLevel(wm->getEnergyLevel() + loadingEnergy);
								wm->setEnergyGained(wm->getEnergyGained() + loadingEnergy);
							}
//                            std::cout << currentAgentWorldModel->getEnergyLevel() << " energy" << std::endl;
						}
					} else {
                        double loadingEnergy = getChangingWorldSupply(NULL, j);
//                        std::cout << "Robot " << i << " is harvesting energy point " << j << " he will get " << loadingEnergy << " energy, and went from " << currentAgentWorldModel->getEnergyLevel() << " to ";

						if (currentAgentWorldModel->getEnergyLevel() + loadingEnergy > ChangingWorldSharedData::maxEnergy) {
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + (ChangingWorldSharedData::maxEnergy - currentAgentWorldModel->getEnergyLevel()));
							currentAgentWorldModel->setEnergyLevel(ChangingWorldSharedData::maxEnergy);
						} else {
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + loadingEnergy);
						}
//                        std::cout << currentAgentWorldModel->getEnergyLevel() << " energy" << std::endl;
					}
                    factory->setActiveStatus(energyPoints[j],false);
				}
			}
		}
	}
}

double ChangingWorldWorldObserver::getChangingWorldSupply(Organism* organism, int powerSupply) {
	int agentsOnScale = 0;
	if(organism){
        agentsOnScale = organism->size();
	}else{
		agentsOnScale = 1;
	}
	return (ChangingWorldSharedData::scaleFactor[ChangingWorldSharedData::currentFactor] * agentsOnScale) + gEnergyPointValue;
}
