#include "Config/ScalePowerSimpleConfigurationLoader.h"

#include "ScalePowerSimple/include/ScalePowerSimpleWorldObserver.h"
#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"
#include "ScalePowerSimple/include/ScalePowerSimpleAgentObserver.h"
#include "ScalePowerSimple/include/ScalePowerSimpleControlArchitecture.h"
#include "ScalePowerSimple/include/ScalePowerSimpleSharedData.h"

#include "World/RobotAgentWorldInterface.h"

ScalePowerSimpleConfigurationLoader::ScalePowerSimpleConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

ScalePowerSimpleConfigurationLoader::~ScalePowerSimpleConfigurationLoader()
{
	//nothing to do
}


WorldObserver* ScalePowerSimpleConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ScalePowerSimpleWorldObserver(wm);
}

RobotAgentWorldModel* ScalePowerSimpleConfigurationLoader::make_RobotAgentWorldModel()
{
	return new ScalePowerSimpleAgentWorldModel();
}

AgentObserver* ScalePowerSimpleConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ScalePowerSimpleAgentObserver(wm);
}

BehaviorControlArchitecture* ScalePowerSimpleConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new ScalePowerSimpleControlArchitecture(wm);
}

RobotAgentWorldInterface* ScalePowerSimpleConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool ScalePowerSimpleConfigurationLoader::readProperties(ExtendedProperties *gProperties)
{
	bool returnValue = true;
	
	std::cout << "Reading properties..." << std::endl;
		
	if (gProperties->hasProperty("evolutionStepsPerCandidate")) {
		convertFromString<int>(EvolutionaryControlArchitecture::stepsPerCandidate, gProperties->getProperty("evolutionStepsPerCandidate"), std::dec);
	} else {
		std::cerr << "[MISSING] evolutionStepsPerCandidate value is missing." << std::endl;
		returnValue = false;
	}
	
	string fitnessCalculation;
	gProperties->checkAndGetPropertyValue("evolutionFitnessCalculation", &fitnessCalculation, true);
	if(fitnessCalculation == "average"){
		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
		EvolutionaryControlArchitecture::fitnessCalculation = EvolutionaryControlArchitecture::AVERAGE;
	}else if(fitnessCalculation == "cumulative"){
		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
		EvolutionaryControlArchitecture::fitnessCalculation = EvolutionaryControlArchitecture::CUMULATIVE;
	}
	
	
	if (gProperties->hasProperty("controllerInitialMutationStepSize")) {
		convertFromString<double>(Controller::initialMutationStepSize, gProperties->getProperty("controllerInitialMutationStepSize"), std::dec);
	} else {
		std::cerr << "[MISSING] controllerInitialMutationStepSize value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("populationMu")) {
		convertFromString<unsigned int>(PopulationControlArchitecture::mu, gProperties->getProperty("populationMu"), std::dec);
	} else {
		std::cerr << "[MISSING] populationMu value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("populationReEvaluationRate")) {
		convertFromString<double>(PopulationControlArchitecture::reEvaluationRate, gProperties->getProperty("populationReEvaluationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationReEvaluationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("populationCrossoverRate")) {
		convertFromString<double>(PopulationControlArchitecture::crossoverRate, gProperties->getProperty("populationCrossoverRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationCrossoverRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("populationMutationRate")) {
		convertFromString<double>(PopulationControlArchitecture::mutationRate, gProperties->getProperty("populationMutationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationMutationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	gProperties->checkAndGetPropertyValue("gScaling", &ScalePowerSimpleSharedData::SCALING, true);
	gProperties->checkAndGetPropertyValue("gMaxEnergy", &ScalePowerSimpleSharedData::MAX_ENERGY, true);
	gProperties->checkAndGetPropertyValue("gStartEnergy", &ScalePowerSimpleSharedData::START_ENERGY, true);

	gProperties->checkAndGetPropertyValue("gSunLifetime", &ScalePowerSimpleSharedData::gSunLifetime, true);
	
	gProperties->checkAndGetPropertyValue("gGenomesLogFilename", &ScalePowerSimpleSharedData::gGenomesLogFilename, true);
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &ScalePowerSimpleSharedData::gOutputLogFilename, true);
	
	cout << "Logging genomes to: " << ScalePowerSimpleSharedData::gGenomesLogFilename << std::endl;
	cout << "Logging output to: " << ScalePowerSimpleSharedData::gOutputLogFilename << std::endl;
	
	cout << "Scaling: " << ScalePowerSimpleSharedData::SCALING << endl;
	cout << "Start Energy: " << ScalePowerSimpleSharedData::START_ENERGY << endl;
	cout << "Max Energy: " << ScalePowerSimpleSharedData::MAX_ENERGY << endl;
	cout << "Sun Lifetime: " << ScalePowerSimpleSharedData::gSunLifetime << endl;
	
	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	
	return returnValue;
}