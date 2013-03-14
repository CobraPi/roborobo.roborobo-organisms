
#include <math.h>

#include "ScalePowerSimple/include/ScalePowerSimpleWorldObserver.h"

#include "World/World.h"
#include "ScalePowerSimple/include/ScalePowerSimpleSharedData.h"
#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "World/EnergyPoint.h"

ScalePowerSimpleWorldObserver::ScalePowerSimpleWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

ScalePowerSimpleWorldObserver::~ScalePowerSimpleWorldObserver() {
	// nothing to do.
}

void ScalePowerSimpleWorldObserver::showOrganisms() {
}

void ScalePowerSimpleWorldObserver::reset() {
	Point2d positionSun0(150, 250);
	Point2d positionSun1(875, 250);

	gEnergyPoints[0].hide();
	gEnergyPoints[0].setFixedLocationStatus(true);
	gEnergyPoints[0].setPosition(positionSun0);

	gEnergyPoints[1].hide();
	gEnergyPoints[1].setFixedLocationStatus(true);
	gEnergyPoints[1].setPosition(positionSun1);
	
	
	if(ScalePowerSimpleSharedData::genomeLogFile){
		ScalePowerSimpleSharedData::genomeLogFile.close();
	}
	ScalePowerSimpleSharedData::genomeLogFile.open(ScalePowerSimpleSharedData::gGenomesLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ScalePowerSimpleSharedData::genomeLogFile) {
		std::cout << "[error] Cannot open log file" << ScalePowerSimpleSharedData::gGenomesLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ScalePowerSimpleSharedData::genomeLogFile << "id,timestep,genome" << std::endl;
	
	if(ScalePowerSimpleSharedData::outputLogFile){
		ScalePowerSimpleSharedData::outputLogFile.close();
	}
	ScalePowerSimpleSharedData::outputLogFile.open(ScalePowerSimpleSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ScalePowerSimpleSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << ScalePowerSimpleSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ScalePowerSimpleSharedData::outputLogFile << "id,timestep,ring" << std::endl;
}

void ScalePowerSimpleWorldObserver::step() {
	updateAllAgentsEnergy();
	int totalStepsPerGeneration = EvolutionaryControlArchitecture::stepsPerCandidate + EvolutionaryControlArchitecture::stepsPerFree + EvolutionaryControlArchitecture::stepsPerRandomFree;
	
	if (gWorld->getIterations() % (int(totalStepsPerGeneration * ScalePowerSimpleSharedData::gSunLifetime)) == 0) {	
		gEnergyPoints[ScalePowerSimpleSharedData::gActiveSun].hide();
		gEnergyPoints[ScalePowerSimpleSharedData::gActiveSun].setActiveStatus(false);
		ScalePowerSimpleSharedData::gActiveSun = (ScalePowerSimpleSharedData::gActiveSun + 1) % 2;
		//std::cout << "\nActive Sun #" << ScalePowerSimpleSharedData::gActiveSun << "\n";
		gEnergyPoints[ScalePowerSimpleSharedData::gActiveSun].display();
		gEnergyPoints[ScalePowerSimpleSharedData::gActiveSun].setActiveStatus(true);
	}
}

void ScalePowerSimpleWorldObserver::updateAllAgentsEnergy() {
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		ScalePowerSimpleAgentWorldModel *currentAgentWorldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (gWorld->getAgent(i)->getWorldModel());

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
						
						double loadingEnergy = getScalePowerSimpleSupply(organism, j);

						// divide the energy evenly
						loadingEnergy = loadingEnergy / organism->size();
						
						std::vector<RobotAgentPtr>::iterator it2;
						for (it2 = organism->agents.begin(); it2 != organism->agents.end(); it2++) {
							ScalePowerSimpleAgentWorldModel* wm = dynamic_cast<ScalePowerSimpleAgentWorldModel*> ((*it2)->getWorldModel());
							if (wm->getEnergyLevel() + loadingEnergy > ScalePowerSimpleSharedData::MAX_ENERGY) {
								wm->setDeltaEnergy(wm->getDeltaEnergy() + (ScalePowerSimpleSharedData::MAX_ENERGY - wm->getEnergyLevel()));
								wm->setEnergyGained(wm->getEnergyGained() + (ScalePowerSimpleSharedData::MAX_ENERGY - wm->getEnergyLevel()));
								wm->setEnergyLevel(ScalePowerSimpleSharedData::MAX_ENERGY);
							} else {
								wm->setEnergyLevel(wm->getEnergyLevel() + loadingEnergy);
								wm->setDeltaEnergy(wm->getDeltaEnergy() + loadingEnergy);
								wm->setEnergyGained(wm->getEnergyGained() + loadingEnergy);
							}
						}
					} else {
						double loadingEnergy = getScalePowerSimpleSupply(NULL, j);
						if (currentAgentWorldModel->getEnergyLevel() + loadingEnergy > ScalePowerSimpleSharedData::MAX_ENERGY) {
							currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + (ScalePowerSimpleSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + (ScalePowerSimpleSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
							currentAgentWorldModel->setEnergyLevel(ScalePowerSimpleSharedData::MAX_ENERGY);
						} else {
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
							currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + loadingEnergy);
						}
					}
				}
			}
		}
	}
}

double ScalePowerSimpleWorldObserver::getScalePowerSimpleSupply(Organism* organism, int powerSupply) {
	int agentsOnScale = 0;
	if(organism){
		for (int i = 0; i < _world->getNbOfAgent(); i++) {
			RobotAgentPtr agent = gWorld->getAgent(i);
			ScalePowerSimpleAgentWorldModel *currentAgentWorldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (agent->getWorldModel());
			Point2d posRobot(currentAgentWorldModel->_xReal,currentAgentWorldModel->_yReal);

			if (organism->contains(agent) && (getEuclidianDistance(posRobot,  gEnergyPoints[powerSupply].getPosition()) < gEnergyPointRadius) && (gEnergyPoints[powerSupply].getActiveStatus())) {
				agentsOnScale++;
			}
		}
	}else{
		agentsOnScale = 1;
	}
	switch (ScalePowerSimpleSharedData::SCALING) {
		case ScalePowerSimpleSharedData::LOGARITHMIC_SCALING:
			return log(agentsOnScale + 1) * 2;
			break;
		case ScalePowerSimpleSharedData::LINEAR_SCALING:
			return (agentsOnScale + 1) * 2;
			break;
		case ScalePowerSimpleSharedData::EXPONENTIAL_SCALING:
			return exp(agentsOnScale + 1) * 2;
			break;
		default:
			return (agentsOnScale + 1) * 2;
	}
}
