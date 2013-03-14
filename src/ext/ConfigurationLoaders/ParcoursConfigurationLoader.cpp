#include "Config/ParcoursConfigurationLoader.h"

#include "Parcours/include/ParcoursWorldObserver.h"
#include "Parcours/include/ParcoursAgentWorldModel.h"
#include "Parcours/include/ParcoursAgentObserver.h"
#include "Parcours/include/ParcoursControlArchitecture.h"
#include "Parcours/include/ParcoursSharedData.h"
#include "Parcours/include/ParcoursWorldInterface.h"

#include "World/RobotAgentWorldInterface.h"

ParcoursConfigurationLoader::ParcoursConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new ParcoursWorldInterface();
	readProperties(&gProperties);
}

ParcoursConfigurationLoader::~ParcoursConfigurationLoader()
{
	//nothing to do
	delete _AgentWorldInterface;
}


WorldObserver* ParcoursConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ParcoursWorldObserver(wm);
}

RobotAgentWorldModel* ParcoursConfigurationLoader::make_RobotAgentWorldModel()
{
	return new ParcoursAgentWorldModel();
}

AgentObserver* ParcoursConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ParcoursAgentObserver(wm);
}

BehaviorControlArchitecture* ParcoursConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new ParcoursControlArchitecture(wm);
}

RobotAgentWorldInterface* ParcoursConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool ParcoursConfigurationLoader::readProperties(ExtendedProperties *gProperties)
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
		convertFromString<double>(ParcoursControlArchitecture::migrationRate, gProperties->getProperty("migrationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] migrationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("bestRate")) {
		convertFromString<double>(ParcoursControlArchitecture::bestRate, gProperties->getProperty("bestRate"), std::dec);
	} else {
		std::cerr << "[MISSING] bestRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("migrationType")) {
		convertFromString<int>(ParcoursControlArchitecture::migrationType, gProperties->getProperty("migrationType"), std::dec);
	} else {
		std::cerr << "[MISSING] migrationType value is missing." << std::endl;
		returnValue = false;
	}
	if (gProperties->hasProperty("topologyType")) {
		convertFromString<int>(ParcoursControlArchitecture::topologyType, gProperties->getProperty("topologyType"), std::dec);
	} else {
		std::cerr << "[MISSING] topologyType value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("eliteSize")) {
		convertFromString<double>(ParcoursControlArchitecture::eliteSize, gProperties->getProperty("eliteSize"), std::dec);
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
	gProperties->checkAndGetPropertyValue("gGenomesLogFilename", &ParcoursSharedData::gGenomesLogFilename, true);
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &ParcoursSharedData::gOutputLogFilename, true);
	gProperties->checkAndGetPropertyValue("gLocationLogFilename", &ParcoursSharedData::gLocationLogFilename, true);
	
//	cout << "Logging output to: " << ParcoursSharedData::gOutputLogFilename << std::endl;
//	
//	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
//	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
//	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
//	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
//	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
//	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	//std::cout << "migration rate: " << ParcoursControlArchitecture::migrationRate << std::endl;
	//std::cout << "bestRate: " << ParcoursControlArchitecture::bestRate << std::endl;
	//std::cout << "eliteSize: " << ParcoursControlArchitecture::eliteSize << std::endl;
	//std::cout << "topologyType: " << ParcoursControlArchitecture::topologyType << std::endl;
	//std::cout << "migrationType: " << ParcoursControlArchitecture::migrationType << std::endl;
	
	return returnValue;
}
