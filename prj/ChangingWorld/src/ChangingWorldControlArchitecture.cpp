
#include "ChangingWorld/include/ChangingWorldControlArchitecture.h"
#include "ChangingWorld/include/ChangingWorldAgentWorldModel.h"
#include "World/World.h"
#include "ChangingWorld/include/ChangingWorldController.h"

double ChangingWorldControlArchitecture::migrationRate;
double ChangingWorldControlArchitecture::eliteSize;
double ChangingWorldControlArchitecture::bestRate;
int ChangingWorldControlArchitecture::migrationType;
int ChangingWorldControlArchitecture::topologyType;

ChangingWorldControlArchitecture::ChangingWorldControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

ChangingWorldControlArchitecture::~ChangingWorldControlArchitecture() {
	// nothing to do.
}

ControllerPtr ChangingWorldControlArchitecture::createRandomGenome() {
	/*
	 * Inputs:
	 * Distance sensors
	 * Ditch sensors
	 * organism size
	 * bias node
	 * 
	 * Outputs:
	 * 5x vote for a strategy
	 * organism size
	 * direction
	 * speed
	 */
	int nbWeights = ((_wm->getDefaultSensors()->getSensorCount() * 2) + 2) * 8;
	vector<double> weights = vector<double>(nbWeights, 0);
	for (int i = 0; i < nbWeights; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}

	return boost::make_shared<ChangingWorldController>(weights);
}

double ChangingWorldControlArchitecture::calculateStepFitness() {
	ChangingWorldAgentWorldModel* worldModel = dynamic_cast<ChangingWorldAgentWorldModel*> (_wm);

//	double distance = getEuclidianDistance(worldModel->_xReal, worldModel->_yReal, worldModel->getXStart(), worldModel->getYStart());
//	double fitness = distance;

    double fitness = worldModel->getEnergyGained();
	return fitness;
}
