
#include <math.h>

#include "ScalePower/include/ScalePowerWorldObserver.h"

#include "World/World.h"
#include "ScalePower/include/ScalePowerSharedData.h"
#include "ScalePower/include/ScalePowerAgentWorldModel.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "World/EnergyPoint.h"

ScalePowerWorldObserver::ScalePowerWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

ScalePowerWorldObserver::~ScalePowerWorldObserver() {
	// nothing to do.
}

void ScalePowerWorldObserver::showOrganisms() {
}

void ScalePowerWorldObserver::reset() {
	Point2d positionSun0(150, 250);
	Point2d positionSun1(875, 250);

	gEnergyPoints[0].hide();
	gEnergyPoints[0].setFixedLocationStatus(true);
	gEnergyPoints[0].setPosition(positionSun0);

	gEnergyPoints[1].hide();
	gEnergyPoints[1].setFixedLocationStatus(true);
	gEnergyPoints[1].setPosition(positionSun1);
	
	
	if(ScalePowerSharedData::genomeLogFile){
		ScalePowerSharedData::genomeLogFile.close();
	}
	ScalePowerSharedData::genomeLogFile.open(ScalePowerSharedData::gGenomesLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ScalePowerSharedData::genomeLogFile) {
		std::cout << "[error] Cannot open log file" << ScalePowerSharedData::gGenomesLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ScalePowerSharedData::genomeLogFile << "id,timestep,weights" << std::endl;
	
	if(ScalePowerSharedData::outputLogFile){
		ScalePowerSharedData::outputLogFile.close();
	}
	ScalePowerSharedData::outputLogFile.open(ScalePowerSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ScalePowerSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << ScalePowerSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ScalePowerSharedData::outputLogFile << "id,timestep,strategy,ring" << std::endl;
}

void ScalePowerWorldObserver::step() {
	updateAllAgentsEnergy();
	int totalStepsPerGeneration = EvolutionaryControlArchitecture::stepsPerCandidate + EvolutionaryControlArchitecture::stepsPerFree + EvolutionaryControlArchitecture::stepsPerRandomFree;
	
	if (gWorld->getIterations() % (int(totalStepsPerGeneration * ScalePowerSharedData::gSunLifetime)) == 0) {	
		gEnergyPoints[ScalePowerSharedData::gActiveSun].hide();
		gEnergyPoints[ScalePowerSharedData::gActiveSun].setActiveStatus(false);
		ScalePowerSharedData::gActiveSun = (ScalePowerSharedData::gActiveSun + 1) % 2;
		//std::cout << "\nActive Sun #" << ScalePowerSharedData::gActiveSun << "\n";
		gEnergyPoints[ScalePowerSharedData::gActiveSun].display();
		gEnergyPoints[ScalePowerSharedData::gActiveSun].setActiveStatus(true);
	}
}

void ScalePowerWorldObserver::updateAllAgentsEnergy() {
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		ScalePowerAgentWorldModel *currentAgentWorldModel = dynamic_cast<ScalePowerAgentWorldModel*> (gWorld->getAgent(i)->getWorldModel());

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
			Point2d posRobot(currentAgentWorldModel->_xReal,currentAgentWorldModel->_yReal);
			//for (std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++) {
			for(unsigned int j =0; j < gEnergyPoints.size();j++){
				
				//if ((getEuclidianDistance(posRobot, it->getPosition()) < gEnergyPointRadius) && (it->getActiveStatus())) {
				if ((getEuclidianDistance(posRobot, gEnergyPoints[j].getPosition()) < gEnergyPointRadius) && (gEnergyPoints[j].getActiveStatus())) {
					// update energy level
					//std::cout << "Energy Point: " << j << " is giving energy to agent: " << currentAgentWorldModel->_agentId << std::endl;
					
					if (gWorld->getAgent(i)->isPartOfOrganism()) {
						Organism* organism = gWorld->getAgent(i)->getOrganism();
						
						double loadingEnergy = getScalePowerSupply(organism, j);

						// divide the energy evenly
						loadingEnergy = loadingEnergy / organism->size();
						
						std::vector<RobotAgentPtr>::iterator it2;
						for (it2 = organism->agents.begin(); it2 != organism->agents.end(); it2++) {
							ScalePowerAgentWorldModel* wm = dynamic_cast<ScalePowerAgentWorldModel*> ((*it2)->getWorldModel());
							if (wm->getEnergyLevel() + loadingEnergy > ScalePowerSharedData::MAX_ENERGY) {
								wm->setDeltaEnergy(loadingEnergy);
								wm->setEnergyGained(wm->getEnergyGained() + (ScalePowerSharedData::MAX_ENERGY - wm->getEnergyLevel()));
								wm->setEnergyLevel(ScalePowerSharedData::MAX_ENERGY);
							} else {
								wm->setEnergyLevel(wm->getEnergyLevel() + loadingEnergy);
								wm->setDeltaEnergy(loadingEnergy);
								wm->setEnergyGained(wm->getEnergyGained() + loadingEnergy);
							}
						}
					} else {
						double loadingEnergy = getScalePowerSupply(NULL, j);
						if (currentAgentWorldModel->getEnergyLevel() + loadingEnergy > ScalePowerSharedData::MAX_ENERGY) {
							currentAgentWorldModel->setDeltaEnergy(loadingEnergy);
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + (ScalePowerSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
							currentAgentWorldModel->setEnergyLevel(ScalePowerSharedData::MAX_ENERGY);
						} else {
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setDeltaEnergy(loadingEnergy);
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + loadingEnergy);
						}
					}
				}
			}
		}
	}
}

double ScalePowerWorldObserver::getScalePowerSupply(Organism* organism, int powerSupply) {
	int agentsOnScale = 0;
	if(organism){
		for (int i = 0; i < _world->getNbOfAgent(); i++) {
			RobotAgentPtr agent = gWorld->getAgent(i);
			ScalePowerAgentWorldModel *currentAgentWorldModel = dynamic_cast<ScalePowerAgentWorldModel*> (agent->getWorldModel());
			Point2d posRobot(currentAgentWorldModel->_xReal,currentAgentWorldModel->_yReal);

			if (organism->contains(agent) && (getEuclidianDistance(posRobot,  gEnergyPoints[powerSupply].getPosition()) < gEnergyPointRadius) && (gEnergyPoints[powerSupply].getActiveStatus())) {
				agentsOnScale++;
			}
		}
	}else{
		agentsOnScale = 1;
	}
	switch (ScalePowerSharedData::SCALING) {
		case ScalePowerSharedData::LOGARITHMIC_SCALING:
			return log(agentsOnScale + 1) * 2;
			break;
		case ScalePowerSharedData::LINEAR_SCALING:
			return (agentsOnScale + 1) * 2;
			break;
		case ScalePowerSharedData::EXPONENTIAL_SCALING:
			return exp(agentsOnScale + 1) * 2;
			break;
		default:
			return (agentsOnScale + 1) * 2;
	}
}
