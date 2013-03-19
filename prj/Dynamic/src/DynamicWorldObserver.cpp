
#include <math.h>
#include <algorithm>
#include "boost/lexical_cast.hpp"

#include "Dynamic/include/DynamicWorldObserver.h"

#include "World/World.h"
#include "Dynamic/include/DynamicSharedData.h"
#include "Dynamic/include/DynamicAgentWorldModel.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "World/TypedEnergyPoint.h"
#include "SDL_collide.h"

typedef ResourceFactory<TypedEnergyPoint>::ResourcePtr TypedEnergyPointPtr;

DynamicWorldObserver::DynamicWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
    _world->setInitializeEnergyPoints(false);
}

DynamicWorldObserver::~DynamicWorldObserver() {
	// nothing to do.
}

void DynamicWorldObserver::reset() {
	if(DynamicSharedData::genomeLogFile){
		DynamicSharedData::genomeLogFile.close();
	}
	DynamicSharedData::genomeLogFile.open(DynamicSharedData::gGenomesLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!DynamicSharedData::genomeLogFile) {
		std::cout << "[error] Cannot open log file" << DynamicSharedData::gGenomesLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	DynamicSharedData::genomeLogFile << "id,timestep,weights" << std::endl;
	
	if(DynamicSharedData::outputLogFile){
		DynamicSharedData::outputLogFile.close();
	}
	DynamicSharedData::outputLogFile.open(DynamicSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!DynamicSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file " << DynamicSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	DynamicSharedData::outputLogFile << "id,timestep,outputs,choice,desired organism size,direction,speed" << std::endl;

    if(DynamicSharedData::locationLogFile){
		DynamicSharedData::locationLogFile.close();
	}
	DynamicSharedData::locationLogFile.open(DynamicSharedData::gLocationLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!DynamicSharedData::locationLogFile) {
		std::cout << "[error] Cannot open log file " << DynamicSharedData::gLocationLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	DynamicSharedData::locationLogFile << "id,timestep,x,y,in org,org_size,swarm time,organism time" << std::endl;
    
    initializePlants();
}


void DynamicWorldObserver::step() {
	int iterations = gWorld->getIterations();
	int totalStepsPerGeneration = EvolutionaryControlArchitecture::stepsPerCandidate + EvolutionaryControlArchitecture::stepsPerFree + EvolutionaryControlArchitecture::stepsPerRandomFree;
    
    if(iterations > 0 && (iterations % (DynamicSharedData::gEnvironmentLifetime * totalStepsPerGeneration ) == 0)){
        updatePlants();
    }
	updateAllAgentsEnergy();
}

void DynamicWorldObserver::initializePlants(){
    _world->registerResourceFactory(ResourceFactory<TypedEnergyPoint>::getInstance());
    
    int totPlants = 0;
    yintercept = 20 - 4 * DynamicSharedData::slope;
//    yintercept = -(DynamicSharedData::NUM_WEIGHTS/2)*DynamicSharedData::slope + DynamicSharedData::fixedPointAmount;
    
    typedEnergyPoints = std::vector<std::vector<ResourceFactory<TypedEnergyPoint>::ResourcePtr > >();
    ResourceFactory<TypedEnergyPoint>::ResourceFactoryPtr factory = ResourceFactory<TypedEnergyPoint>::getInstance();
    for (int i=0;i<DynamicSharedData::NUM_WEIGHTS;i++){
        int nPlants = DynamicSharedData::slope * i + yintercept;
        std::vector<ResourceFactory<TypedEnergyPoint>::ResourcePtr > vec;
        typedEnergyPoints.push_back(vec);
        for(int j=0;j<nPlants;j++){
            ResourceFactory<TypedEnergyPoint>::ResourcePtr point = factory->addResource();
            point->setType(i);
        	typedEnergyPoints[i].push_back(point);
            totPlants++;
        }
    }
    factory->displayResources();
    if(totPlants != DynamicSharedData::NUM_PLANTS){
        std::cerr << "We've created more plants than expected: " << totPlants << " vs. " << DynamicSharedData::NUM_PLANTS << std::endl;
    }
}

void DynamicWorldObserver::updatePlants(){
    double newSlope = DynamicSharedData::slope + DynamicSharedData::slopeChange;
    int newYintercept = 20 - 4 * newSlope;
//    yintercept = -(DynamicSharedData::NUM_WEIGHTS/2)*DynamicSharedData::slope + DynamicSharedData::fixedPointAmount;
    
    int minimum = newYintercept;
    if((newSlope * (DynamicSharedData::NUM_WEIGHTS-1)) + newYintercept < minimum){
        minimum = (newSlope * (DynamicSharedData::NUM_WEIGHTS-1)) + newYintercept;
    }
    
    if(DynamicSharedData::reverse && minimum <= 0){
        DynamicSharedData::slopeChange = -DynamicSharedData::slopeChange;
    }else if(minimum <= 0){
        DynamicSharedData::slopeChange = 0;
    }
    
    DynamicSharedData::slope = DynamicSharedData::slope + DynamicSharedData::slopeChange;
    yintercept = 20 - 4 * DynamicSharedData::slope;
    
    std::cerr << "Slope: " << DynamicSharedData::slope << " y-intercept: " << yintercept << std::endl;
    std::cerr << "Plants per type:";
    int totPlants = 0;
    int sizes[DynamicSharedData::NUM_WEIGHTS];

    // Update the number of plants
    ResourceFactory<TypedEnergyPoint>::ResourceFactoryPtr factory = ResourceFactory<TypedEnergyPoint>::getInstance();
    for (int i=0;i<DynamicSharedData::NUM_WEIGHTS;i++){
        int nPlants = DynamicSharedData::slope * i + yintercept;
        if(typedEnergyPoints[i].size() < nPlants){
            int newPlants = nPlants - typedEnergyPoints[i].size();
            for(int j=0;j<newPlants;j++){
                TypedEnergyPointPtr point = factory->addResource();
                point->setType(i);
            }
        }else if(typedEnergyPoints[i].size() > nPlants){
            int remPlants = typedEnergyPoints[i].size() - nPlants;
            for(int j=0;j<remPlants;j++){
            	typedEnergyPoints[i].back()->setActiveStatus(false);
                factory->removeResource(typedEnergyPoints[i].back());
                typedEnergyPoints[i].pop_back();
            }
        }
        totPlants += typedEnergyPoints[i].size();
        sizes[i] = typedEnergyPoints[i].size();
        std::cerr << " " << i << ": " << typedEnergyPoints[i].size();
    }
    std::cerr << std::endl;
  
    // Check if we have exactly NUM_PLANTS plants
    // otherwise add or remove from lowest/highest amount
    if(totPlants > DynamicSharedData::NUM_PLANTS){
        int excess = totPlants-DynamicSharedData::NUM_PLANTS;
        if(sizes[0] > sizes[DynamicSharedData::NUM_WEIGHTS-1]){
            int index = 0;
            for(int i=0;i<excess;i++){
                typedEnergyPoints[i].back()->setActiveStatus(false);
                factory->removeResource(typedEnergyPoints[i].back());
            	typedEnergyPoints[index].pop_back();
                index = (index + 1) % DynamicSharedData::NUM_WEIGHTS;
                totPlants--;
            }
        }else{
            int index = DynamicSharedData::NUM_WEIGHTS-1;
            for(int i=0;i<excess;i++){
                typedEnergyPoints[i].back()->setActiveStatus(false);
                factory->removeResource(typedEnergyPoints[i].back());
            	typedEnergyPoints[index].pop_back();
                
                index = (index - 1) % DynamicSharedData::NUM_WEIGHTS;
                totPlants--;
            }
        }
    } else if(totPlants < DynamicSharedData::NUM_PLANTS){
        int lack = DynamicSharedData::NUM_PLANTS - totPlants;
        if(sizes[0] < sizes[DynamicSharedData::NUM_WEIGHTS-1]){
            int index = 0;
            for(int i=0;i<lack;i++){
                TypedEnergyPointPtr point = factory->addResource();
                point->setType(i);
                index = (index + 1) % DynamicSharedData::NUM_WEIGHTS;
                totPlants++;
            }
        }else{
            int index = DynamicSharedData::NUM_WEIGHTS-1;
            for(int i=0;i<lack;i++){
                TypedEnergyPointPtr point = factory->addResource();
                point->setType(i);
                index = (index - 1) % DynamicSharedData::NUM_WEIGHTS;
                totPlants++;
            }
        }
    }
    if(totPlants != DynamicSharedData::NUM_PLANTS){
        std::cerr << "We've created a different amount of plants than expected: " << totPlants << " vs. " << DynamicSharedData::NUM_PLANTS << std::endl;
    }
}

void DynamicWorldObserver::updateAllAgentsEnergy() {
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		DynamicAgentWorldModel *currentAgentWorldModel = dynamic_cast<DynamicAgentWorldModel*> (gWorld->getAgent(i)->getWorldModel());

		currentAgentWorldModel->setDeltaEnergy(0);
		currentAgentWorldModel->setEnergyGained(0);

		// * check energy level. Becomes inactive if zero.
		if (currentAgentWorldModel->getEnergyLevel() <= 0) {
			currentAgentWorldModel->setEnergyLevel(0);
			currentAgentWorldModel->setDeltaEnergy(0);
			currentAgentWorldModel->setActive(false);
		}

		if (currentAgentWorldModel->getEnergyLevel() > 0.0) {
			currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() - 1);
		} else {
			currentAgentWorldModel->setEnergyLevel(0);
		}
		currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() - 1);

		if (currentAgentWorldModel->isActive()) {
			Point2d posRobot = currentAgentWorldModel->getPosition();
			for(unsigned int j = 0; j < typedEnergyPoints.size();j++){
                for(unsigned int k = 0; k < typedEnergyPoints[j].size();k++){
                    if (typedEnergyPoints[j][k]->getActiveStatus() && // The energy point is active
                        posRobot.distance(typedEnergyPoints[j][k]->getPosition()) < gEnergyPointRadius // the robot is on it
                        ) {
                        // update energy level
                        //std::cout << "Energy Point: " << j << " is giving energy to agent: " << currentAgentWorldModel->_agentId << std::endl;
                        int plantType = typedEnergyPoints[j][k]->getType();
                        if(plantType < 1 // either the plant is edible for all
                           || (gWorld->getAgent(i)->isPartOfOrganism() && gWorld->getAgent(i)->getOrganism()->size() > plantType) // or the organism is larger than the plants size
                           ) {
                            double loadingEnergy = (typedEnergyPoints[j][k]->getType()+1) * gEnergyPointValue;
                            if (gWorld->getAgent(i)->isPartOfOrganism()) {
                                Organism* organism = gWorld->getAgent(i)->getOrganism();

                                // divide the energy evenly
                                loadingEnergy = loadingEnergy / organism->size();

                                std::vector<RobotAgentPtr>::iterator it2;

                                for (it2 = organism->agents.begin(); it2 != organism->agents.end(); it2++) {
                                    DynamicAgentWorldModel* wm = dynamic_cast<DynamicAgentWorldModel*> ((*it2)->getWorldModel());
                                    if (wm->getEnergyLevel() + loadingEnergy > DynamicSharedData::MAX_ENERGY) {
                                        wm->setDeltaEnergy(loadingEnergy);
                                        wm->setEnergyGained(wm->getEnergyGained() + (DynamicSharedData::MAX_ENERGY - wm->getEnergyLevel()));
                                        wm->setEnergyLevel(DynamicSharedData::MAX_ENERGY);
                                    } else {
                                        wm->setEnergyLevel(wm->getEnergyLevel() + loadingEnergy);
                                        wm->setDeltaEnergy(loadingEnergy);
                                        wm->setEnergyGained(wm->getEnergyGained() + loadingEnergy);
                                    }
                                }
                            } else {
                                if (currentAgentWorldModel->getEnergyLevel() + loadingEnergy > DynamicSharedData::MAX_ENERGY) {
                                    currentAgentWorldModel->setDeltaEnergy(loadingEnergy);
                                    currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + (DynamicSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
                                    currentAgentWorldModel->setEnergyLevel(DynamicSharedData::MAX_ENERGY);
                                } else {
                                    currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
                                    currentAgentWorldModel->setDeltaEnergy(loadingEnergy);
                                    currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + loadingEnergy);
                                }
                            }
                            typedEnergyPoints[j][k]->setActiveStatus(false);
                        }
                    }
				}
			}
		}
    }
}
