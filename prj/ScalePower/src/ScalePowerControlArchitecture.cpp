
#include "ScalePower/include/ScalePowerControlArchitecture.h"
#include "ScalePower/include/ScalePowerAgentWorldModel.h"
#include "World/World.h"
#include "ScalePower/include/ScalePowerController.h"

ScalePowerControlArchitecture::ScalePowerControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

ScalePowerControlArchitecture::~ScalePowerControlArchitecture() {
	// nothing to do.
}

ControllerPtr ScalePowerControlArchitecture::createRandomGenome() {
	/*
	 * Inputs:
	 * Sensors
	 * organism size
	 * angle to goal
	 * distance to goal
	 * energy level
	 * 1 bias node
	 * 
	 * Outputs:
	 * 4x vote for a strategy
	 * 1x Connection setting
	 */
	int nbWeights = (_wm->getDefaultSensors()->getSensorCount() + 5) * 8;
	vector<double> weights = vector<double>(nbWeights, 0);
	for (int i = 0; i < nbWeights; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}

	return boost::make_shared<ScalePowerController>(weights);
}

double ScalePowerControlArchitecture::calculateStepFitness() {
	ScalePowerAgentWorldModel* worldModel = dynamic_cast<ScalePowerAgentWorldModel*> (_wm);
	double fitness = worldModel->getEnergyGained();

	return fitness;
}
