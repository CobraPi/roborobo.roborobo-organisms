#include "Config/ChangeDetectionConfigurationLoader.h"

#include "ChangeDetection/include/ChangeDetectionWorldObserver.h"
#include "ChangeDetection/include/ChangeDetectionAgentWorldModel.h"
#include "ChangeDetection/include/ChangeDetectionAgentObserver.h"
#include "ChangeDetection/include/ChangeDetectionControlArchitecture.h"
#include "ChangeDetection/include/ChangeDetectionSharedData.h"
#include "ChangeDetection/include/ChangeDetectionWorldInterface.h"

#include "World/RobotAgentWorldInterface.h"

ChangeDetectionConfigurationLoader::ChangeDetectionConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new ChangeDetectionWorldInterface();
	readProperties(&gProperties);
}

ChangeDetectionConfigurationLoader::~ChangeDetectionConfigurationLoader()
{
	//nothing to do
	delete _AgentWorldInterface;
}


WorldObserver* ChangeDetectionConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ChangeDetectionWorldObserver(wm);
}

RobotAgentWorldModel* ChangeDetectionConfigurationLoader::make_RobotAgentWorldModel()
{
	return new ChangeDetectionAgentWorldModel();
}

AgentObserver* ChangeDetectionConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ChangeDetectionAgentObserver(wm);
}

BehaviorControlArchitecture* ChangeDetectionConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new ChangeDetectionControlArchitecture(wm);
}

RobotAgentWorldInterface* ChangeDetectionConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool ChangeDetectionConfigurationLoader::readProperties(ExtendedProperties *gProperties)
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
	
	/*if (gProperties->hasProperty("migrationRate")) {
		convertFromString<double>(ChangeDetectionControlArchitecture::migrationRate, gProperties->getProperty("migrationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] migrationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("bestRate")) {
		convertFromString<double>(ChangeDetectionControlArchitecture::bestRate, gProperties->getProperty("bestRate"), std::dec);
	} else {
		std::cerr << "[MISSING] bestRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("migrationType")) {
		convertFromString<int>(ChangeDetectionControlArchitecture::migrationType, gProperties->getProperty("migrationType"), std::dec);
	} else {
		std::cerr << "[MISSING] migrationType value is missing." << std::endl;
		returnValue = false;
	}
	if (gProperties->hasProperty("topologyType")) {
		convertFromString<int>(ChangeDetectionControlArchitecture::topologyType, gProperties->getProperty("topologyType"), std::dec);
	} else {
		std::cerr << "[MISSING] topologyType value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("eliteSize")) {
		convertFromString<double>(ChangeDetectionControlArchitecture::eliteSize, gProperties->getProperty("eliteSize"), std::dec);
	} else {
		std::cerr << "[MISSING] eliteSize value is missing." << std::endl;
		returnValue = false;
	}*/
	
	if (gProperties->hasProperty("populationNumberOfParents")) {
		convertFromString<int>(PopulationControlArchitecture::numberOfParents, gProperties->getProperty("populationNumberOfParents"), std::dec);
	} else {
		std::cerr << "[MISSING] populationNumberOfParents value is missing." << std::endl;
		returnValue = false;
	}
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &ChangeDetectionSharedData::gOutputLogFilename, true);
	gProperties->checkAndGetPropertyValue("gLocationLogFilename", &ChangeDetectionSharedData::gLocationLogFilename, true);
	
//	cout << "Logging output to: " << ChangeDetectionSharedData::gOutputLogFilename << std::endl;
//	
//	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
//	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
//	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
//	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
//	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
//	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	//std::cout << "migration rate: " << ChangeDetectionControlArchitecture::migrationRate << std::endl;
	//std::cout << "bestRate: " << ChangeDetectionControlArchitecture::bestRate << std::endl;
	//std::cout << "eliteSize: " << ChangeDetectionControlArchitecture::eliteSize << std::endl;
	//std::cout << "topologyType: " << ChangeDetectionControlArchitecture::topologyType << std::endl;
	//std::cout << "migrationType: " << ChangeDetectionControlArchitecture::migrationType << std::endl;
	
	return returnValue;
}
