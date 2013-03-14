
#include "ScalePowerSimple/include/ScalePowerSimpleControlArchitecture.h"
#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"
#include "World/World.h"
#include "ScalePowerSimple/include/ScalePowerSimpleController.h"



ScalePowerSimpleControlArchitecture::ScalePowerSimpleControlArchitecture(RobotAgentWorldModel* __wm) : PopulationControlArchitecture(__wm) {
	setPopulation(new HiveMindInstance());
}

ScalePowerSimpleControlArchitecture::~ScalePowerSimpleControlArchitecture() {
	// nothing to do.
}

ControllerPtr ScalePowerSimpleControlArchitecture::createRandomGenome() {
	vector<double> genome = vector<double>(2);
	
	genome[0] = 0; // chance positive
	genome[1] = 0; // chance negative
	
	ControllerPtr controller(new ScalePowerSimpleController(genome));
	controller->mutate();
	return controller;
}

double ScalePowerSimpleControlArchitecture::calculateStepFitness() {
	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);
	double fitness = worldModel->getEnergyGained();

	return fitness;
}
