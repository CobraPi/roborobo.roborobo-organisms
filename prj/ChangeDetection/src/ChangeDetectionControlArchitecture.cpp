
#include "ChangeDetection/include/ChangeDetectionControlArchitecture.h"
#include "ChangeDetection/include/ChangeDetectionAgentWorldModel.h"
#include "World/World.h"
#include "ChangeDetection/include/ChangeDetectionController.h"

double ChangeDetectionControlArchitecture::migrationRate;
double ChangeDetectionControlArchitecture::eliteSize;
double ChangeDetectionControlArchitecture::bestRate;
int ChangeDetectionControlArchitecture::migrationType;
int ChangeDetectionControlArchitecture::topologyType;

ChangeDetectionControlArchitecture::ChangeDetectionControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

ChangeDetectionControlArchitecture::~ChangeDetectionControlArchitecture() {
	// nothing to do.
}

ControllerPtr ChangeDetectionControlArchitecture::createRandomGenome() {
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

	return boost::make_shared<ChangeDetectionController>(weights);
}

double ChangeDetectionControlArchitecture::calculateStepFitness() {
	ChangeDetectionAgentWorldModel* worldModel = dynamic_cast<ChangeDetectionAgentWorldModel*> (_wm);

	double distance = worldModel->getPosition().x - worldModel->xStart;
	double fitness = distance;

	return fitness;
}
