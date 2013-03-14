
#include "Dynamic/include/DynamicControlArchitecture.h"
#include "Dynamic/include/DynamicAgentWorldModel.h"
#include "Dynamic/include/DynamicController.h"
#include "World/World.h"

int DynamicControlArchitecture::cacheSize = 4; // This should be changed by the properties file
int DynamicControlArchitecture::itemTTL = 2; // This should be changed by the properties file

DynamicControlArchitecture::DynamicControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
//	setPopulation(new NewscastAgentGroup(cacheSize, itemTTL, cacheSize));
    setPopulation(new HiveMindInstance());
}

DynamicControlArchitecture::~DynamicControlArchitecture() {
	// nothing to do.
}

ControllerPtr DynamicControlArchitecture::createRandomGenome() {
	/*
	 * Inputs:
	 * Distance Sensors x5
     * Energy Sensors x3
     * Energy Value Sensor x3
	 * organism size
	 * energy level
	 * 1 bias node
	 * 
     // Node 1: Form Organism
     // Node 2: Leave Organism
     // Node 3: Halt
     // Node 4: Avoid obstacle
     // Node 5: Move to direction
     // Node 6: Desired organism size - for node 1
     // Node 7: Desired direction - for node 5
     // Node 8: Desired speed - for node 5
	 */
	int nbWeights = 14 * 8;
	vector<double> weights = vector<double>(nbWeights, 0);
	for (int i = 0; i < nbWeights; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}

	return boost::make_shared<DynamicController>(weights);
}

double DynamicControlArchitecture::calculateStepFitness() {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	double fitness = worldModel->getEnergyGained();
    
	return fitness;
}
