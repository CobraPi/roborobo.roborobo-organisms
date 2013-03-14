#include "Config/ScalePowerConfigurationLoader.h"

#include "ScalePower/include/ScalePowerWorldObserver.h"
#include "ScalePower/include/ScalePowerAgentWorldModel.h"
#include "ScalePower/include/ScalePowerAgentObserver.h"
#include "ScalePower/include/ScalePowerControlArchitecture.h"
#include "ScalePower/include/ScalePowerSharedData.h"

#include "World/RobotAgentWorldInterface.h"

ScalePowerConfigurationLoader::ScalePowerConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

ScalePowerConfigurationLoader::~ScalePowerConfigurationLoader()
{
	//nothing to do
	delete _AgentWorldInterface;
}


WorldObserver* ScalePowerConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ScalePowerWorldObserver(wm);
}

RobotAgentWorldModel* ScalePowerConfigurationLoader::make_RobotAgentWorldModel()
{
	return new ScalePowerAgentWorldModel();
}

AgentObserver* ScalePowerConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ScalePowerAgentObserver(wm);
}

BehaviorControlArchitecture* ScalePowerConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new ScalePowerControlArchitecture(wm);
}

RobotAgentWorldInterface* ScalePowerConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool ScalePowerConfigurationLoader::readProperties(ExtendedProperties *gProperties)
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
	
	gProperties->checkAndGetPropertyValue("gScaling", &ScalePowerSharedData::SCALING, true);
	gProperties->checkAndGetPropertyValue("gMaxEnergy", &ScalePowerSharedData::MAX_ENERGY, true);
	gProperties->checkAndGetPropertyValue("gStartEnergy", &ScalePowerSharedData::START_ENERGY, true);

	gProperties->checkAndGetPropertyValue("gSunLifetime", &ScalePowerSharedData::gSunLifetime, true);
	
	gProperties->checkAndGetPropertyValue("gGenomesLogFilename", &ScalePowerSharedData::gGenomesLogFilename, true);
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &ScalePowerSharedData::gOutputLogFilename, true);
	
	cout << "Logging genomes to: " << ScalePowerSharedData::gGenomesLogFilename << std::endl;
	cout << "Logging output to: " << ScalePowerSharedData::gOutputLogFilename << std::endl;
	
	cout << "Scaling: " << ScalePowerSharedData::SCALING << endl;
	cout << "Start Energy: " << ScalePowerSharedData::START_ENERGY << endl;
	cout << "Max Energy: " << ScalePowerSharedData::MAX_ENERGY << endl;
	cout << "Sun Lifetime: " << ScalePowerSharedData::gSunLifetime << endl;
	
	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	
	return returnValue;
}
