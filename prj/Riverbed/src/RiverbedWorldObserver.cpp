
#include <math.h>

#include "Riverbed/include/RiverbedWorldObserver.h"

#include "World/World.h"
#include "Riverbed/include/RiverbedSharedData.h"
#include "Riverbed/include/RiverbedAgentWorldModel.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "World/EnergyPoint.h"

RiverbedWorldObserver::RiverbedWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

RiverbedWorldObserver::~RiverbedWorldObserver() {
	// nothing to do.
}

void RiverbedWorldObserver::showOrganisms() {
}

void RiverbedWorldObserver::reset() {
	if(RiverbedSharedData::genomeLogFile){
		RiverbedSharedData::genomeLogFile.close();
	}
	RiverbedSharedData::genomeLogFile.open(RiverbedSharedData::gGenomesLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!RiverbedSharedData::genomeLogFile) {
		std::cout << "[error] Cannot open log file" << RiverbedSharedData::gGenomesLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	RiverbedSharedData::genomeLogFile << "id,timestep,weights" << std::endl;
	
	if(RiverbedSharedData::outputLogFile){
		RiverbedSharedData::outputLogFile.close();
	}
	RiverbedSharedData::outputLogFile.open(RiverbedSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!RiverbedSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << RiverbedSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	RiverbedSharedData::outputLogFile << "id,timestep,strategy,ring" << std::endl;
}

void RiverbedWorldObserver::step() {
	updateAllAgentsEnergy();
}

void RiverbedWorldObserver::updateAllAgentsEnergy() {
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		RiverbedAgentWorldModel *currentAgentWorldModel = dynamic_cast<RiverbedAgentWorldModel*> (gWorld->getAgent(i)->getWorldModel());

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
			if (gWorld->getAgent(i)->isPartOfOrganism()) {
				Organism* organism = gWorld->getAgent(i)->getOrganism();

				double loadingEnergy = getRiverbedSupply(organism, gWorld->getAgent(i));

				// divide the energy evenly
				loadingEnergy = loadingEnergy / organism->size();

				std::vector<RobotAgentPtr>::iterator it2;
				for (it2 = organism->agents.begin(); it2 != organism->agents.end(); it2++) {
					RiverbedAgentWorldModel* wm = dynamic_cast<RiverbedAgentWorldModel*> ((*it2)->getWorldModel());
					if (wm->getEnergyLevel() + loadingEnergy > RiverbedSharedData::MAX_ENERGY) {
						wm->setDeltaEnergy(wm->getDeltaEnergy() + (RiverbedSharedData::MAX_ENERGY - wm->getEnergyLevel()));
						wm->setEnergyGained(wm->getEnergyGained() + (RiverbedSharedData::MAX_ENERGY - wm->getEnergyLevel()));
						wm->setEnergyLevel(RiverbedSharedData::MAX_ENERGY);
					} else {
						wm->setEnergyLevel(wm->getEnergyLevel() + loadingEnergy);
						wm->setDeltaEnergy(wm->getDeltaEnergy() + loadingEnergy);
						wm->setEnergyGained(wm->getEnergyGained() + loadingEnergy);
					}
				}
			} else {
				double loadingEnergy = getRiverbedSupply(NULL, gWorld->getAgent(i));
				if (currentAgentWorldModel->getEnergyLevel() + loadingEnergy > RiverbedSharedData::MAX_ENERGY) {
					currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + (RiverbedSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
					currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + (RiverbedSharedData::MAX_ENERGY - currentAgentWorldModel->getEnergyLevel()));
					currentAgentWorldModel->setEnergyLevel(RiverbedSharedData::MAX_ENERGY);
				} else {
					currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
					currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
					currentAgentWorldModel->setEnergyGained(currentAgentWorldModel->getEnergyGained() + loadingEnergy);
				}
			}
		}
	}
}

double RiverbedWorldObserver::getRiverbedSupply(Organism* organism, RobotAgentPtr agent) {
	double position = gEnvironmentImage->w;
	if (!organism) {
		position = gEnvironmentImage->w - agent->getWorldModel()->_xReal;
	} else {
		std::vector<RobotAgentPtr>::iterator it;
		position = 0;
		for (it = organism->agents.begin(); it != organism->agents.end(); it++) {
			position += (*it)->getWorldModel()->_xReal;
		}
		position /= organism->agents.size();
		position = gEnvironmentImage->w - position;
	}
	
	position = position / gEnvironmentImage->w;
	position *= 2;
	
	if(organism){
		position *= organism->agents.size();
	}
	
	switch (RiverbedSharedData::SCALING) {
		case RiverbedSharedData::LOGARITHMIC_SCALING:
			return log(position+1);
			break;
		case RiverbedSharedData::LINEAR_SCALING:
			return (position);
			break;
		case RiverbedSharedData::EXPONENTIAL_SCALING:
			return exp(position);
			break;
		default:
			return (position);
	}
}
