#include "Config/DynamicConfigurationLoader.h"

#include "Dynamic/include/DynamicWorldObserver.h"
#include "Dynamic/include/DynamicAgentWorldModel.h"
#include "Dynamic/include/DynamicAgentObserver.h"
#include "Dynamic/include/DynamicControlArchitecture.h"
#include "Dynamic/include/DynamicSharedData.h"

#include "World/RobotAgentWorldInterface.h"

DynamicConfigurationLoader::DynamicConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

DynamicConfigurationLoader::~DynamicConfigurationLoader()
{
	//nothing to do
	delete _AgentWorldInterface;
}


WorldObserver* DynamicConfigurationLoader::make_WorldObserver(World* wm)
{
	return new DynamicWorldObserver(wm);
}

RobotAgentWorldModel* DynamicConfigurationLoader::make_RobotAgentWorldModel()
{
	return new DynamicAgentWorldModel();
}

AgentObserver* DynamicConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new DynamicAgentObserver(wm);
}

BehaviorControlArchitecture* DynamicConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new DynamicControlArchitecture(wm);
}

RobotAgentWorldInterface* DynamicConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool DynamicConfigurationLoader::readProperties(ExtendedProperties *gProperties)
{
	bool returnValue = true;
	
//	std::cout << "Reading properties..." << std::endl;
		
	if (gProperties->hasProperty("evolutionStepsPerCandidate")) {
		convertFromString<int>(EvolutionaryControlArchitecture::stepsPerCandidate, gProperties->getProperty("evolutionStepsPerCandidate"), std::dec);
	} else {
		std::cerr << "[MISSING] evolutionStepsPerCandidate value is missing." << std::endl;
		returnValue = false;
	}
	
	string fitnessCalculation;
	gProperties->checkAndGetPropertyValue("evolutionFitnessCalculation", &fitnessCalculation, true);
	if(fitnessCalculation == "average"){
//		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
		EvolutionaryControlArchitecture::fitnessCalculation = EvolutionaryControlArchitecture::AVERAGE;
	}else if(fitnessCalculation == "cumulative"){
//		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
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
    
    if (gProperties->hasProperty("newscastCacheSize")) {
		convertFromString<int>(DynamicControlArchitecture::cacheSize, gProperties->getProperty("newscastCacheSize"), std::dec);
	} else {
		std::cerr << "[MISSING] newscastCacheSize value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("newscastItemTTL")) {
		convertFromString<int>(DynamicControlArchitecture::itemTTL, gProperties->getProperty("newscastItemTTL"), std::dec);
	} else {
		std::cerr << "[MISSING] newscastItemTTL value is missing." << std::endl;
		returnValue = false;
	}
	
	gProperties->checkAndGetPropertyValue("gScaling", &DynamicSharedData::SCALING, true);
	gProperties->checkAndGetPropertyValue("gMaxEnergy", &DynamicSharedData::MAX_ENERGY, true);
	gProperties->checkAndGetPropertyValue("gStartEnergy", &DynamicSharedData::START_ENERGY, true);
	
	gProperties->checkAndGetPropertyValue("gGenomesLogFilename", &DynamicSharedData::gGenomesLogFilename, true);
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &DynamicSharedData::gOutputLogFilename, true);
	
	// Jan-Willem: Added properties
	gProperties->checkAndGetPropertyValue("gEnvironmentLifetime", &DynamicSharedData::gEnvironmentLifetime, true);
    gProperties->checkAndGetPropertyValue("gLocationLogFilename", &DynamicSharedData::gLocationLogFilename, true);
    
    gProperties->checkAndGetPropertyValue("numPlants", &DynamicSharedData::NUM_PLANTS, true);
    gProperties->checkAndGetPropertyValue("numWeights", &DynamicSharedData::NUM_WEIGHTS, true);
    gProperties->checkAndGetPropertyValue("slope", &DynamicSharedData::slope, true);
    gProperties->checkAndGetPropertyValue("slopeChange", &DynamicSharedData::slopeChange, true);
    gProperties->checkAndGetPropertyValue("reverse", &DynamicSharedData::reverse, true);

//	cout << "Logging genomes to: " << DynamicSharedData::gGenomesLogFilename << std::endl;
//	cout << "Logging output to: " << DynamicSharedData::gOutputLogFilename << std::endl;
//	
//	cout << "Scaling: " << DynamicSharedData::SCALING << endl;
//	cout << "Start Energy: " << DynamicSharedData::START_ENERGY << endl;
//	cout << "Max Energy: " << DynamicSharedData::MAX_ENERGY << endl;
//	
//	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
//	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
//	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
//	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
//	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
//	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
//	
//	std::cout << "Scenario: " << DynamicSharedData::gScenario << std::endl;

	return returnValue;
}
