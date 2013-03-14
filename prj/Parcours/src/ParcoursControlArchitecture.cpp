
#include "Parcours/include/ParcoursControlArchitecture.h"
#include "Parcours/include/ParcoursAgentWorldModel.h"
#include "World/World.h"
#include "Parcours/include/ParcoursController.h"

double ParcoursControlArchitecture::migrationRate;
double ParcoursControlArchitecture::eliteSize;
double ParcoursControlArchitecture::bestRate;
int ParcoursControlArchitecture::migrationType;
int ParcoursControlArchitecture::topologyType;

ParcoursControlArchitecture::ParcoursControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

ParcoursControlArchitecture::~ParcoursControlArchitecture() {
	// nothing to do.
}

ControllerPtr ParcoursControlArchitecture::createRandomGenome() {
	/*
	 * Inputs:
	 * Distance sensors
	 * Ditch sensors
	 * organism size
	 * angle to goal
	 * distance to goal
	 * bias node
	 * 
	 * Outputs:
	 * 5x vote for a strategy
	 * organism size
	 * direction
	 * speed
	 */
	int nbWeights = ((_wm->getDefaultSensors()->getSensorCount() * 2) + 4) * 8;
	vector<double> weights = vector<double>(nbWeights, 0);
	for (int i = 0; i < nbWeights; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}

	return boost::make_shared<ParcoursController>(weights);
}

double ParcoursControlArchitecture::calculateStepFitness() {
	ParcoursAgentWorldModel* worldModel = dynamic_cast<ParcoursAgentWorldModel*> (_wm);

	double distance = worldModel->_xReal - worldModel->xStart;
	double fitness = distance;

	return fitness;
}
