
#include "Riverbed/include/RiverbedControlArchitecture.h"
#include "Riverbed/include/RiverbedAgentWorldModel.h"
#include "Riverbed/include/RiverbedController.h"
#include "World/World.h"

RiverbedControlArchitecture::RiverbedControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

RiverbedControlArchitecture::~RiverbedControlArchitecture() {
	// nothing to do.
}

ControllerPtr RiverbedControlArchitecture::createRandomGenome() {
	/*
	 * Inputs:
	 * Sensors
	 * organism size
	 * angle to nearest feeding ground
	 * distance to nearest feeding ground
	 * energy level
	 * bias node
	 * 
	 * Outputs:
	 * Angle
	 * Speed
	 * Connection setting
	 */
	int nbWeights = ((_wm->getDefaultSensors()->getSensorCount() + 5) * 8) + (8 * 3);
	vector<double> weights = vector<double>(nbWeights, 0);
	for (int i = 0; i < nbWeights; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}

	return ControllerPtr(new RiverbedController(weights));
}

double RiverbedControlArchitecture::calculateStepFitness() {
	RiverbedAgentWorldModel* worldModel = dynamic_cast<RiverbedAgentWorldModel*> (_wm);
	double fitness = worldModel->getEnergyGained();

	return fitness;
}
