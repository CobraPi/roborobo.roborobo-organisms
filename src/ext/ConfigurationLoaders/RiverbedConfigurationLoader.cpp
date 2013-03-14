#include "Config/RiverbedConfigurationLoader.h"

#include "Riverbed/include/RiverbedWorldObserver.h"
#include "Riverbed/include/RiverbedAgentWorldModel.h"
#include "Riverbed/include/RiverbedAgentObserver.h"
#include "Riverbed/include/RiverbedControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"
#include "Riverbed/include/RiverbedSharedData.h"

RiverbedConfigurationLoader::RiverbedConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties();
}

RiverbedConfigurationLoader::~RiverbedConfigurationLoader()
{
	//nothing to do
}


WorldObserver* RiverbedConfigurationLoader::make_WorldObserver(World* wm)
{
	return new RiverbedWorldObserver(wm);
}

RobotAgentWorldModel* RiverbedConfigurationLoader::make_RobotAgentWorldModel()
{
	return new RiverbedAgentWorldModel();
}

AgentObserver* RiverbedConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new RiverbedAgentObserver(wm);
}

BehaviorControlArchitecture* RiverbedConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new RiverbedControlArchitecture(wm);
}

RobotAgentWorldInterface* RiverbedConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool RiverbedConfigurationLoader::readProperties()
{
	bool returnValue = true;
	
	std::cout << "Reading properties..." << std::endl;
		
	if (gProperties.hasProperty("evolutionStepsPerCandidate")) {
		convertFromString<int>(EvolutionaryControlArchitecture::stepsPerCandidate, gProperties.getProperty("evolutionStepsPerCandidate"), std::dec);
	} else {
		std::cerr << "[MISSING] evolutionStepsPerCandidate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("controllerInitialMutationStepSize")) {
		convertFromString<double>(Controller::initialMutationStepSize, gProperties.getProperty("controllerInitialMutationStepSize"), std::dec);
	} else {
		std::cerr << "[MISSING] controllerInitialMutationStepSize value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("populationMu")) {
		convertFromString<unsigned int>(PopulationControlArchitecture::mu, gProperties.getProperty("populationMu"), std::dec);
	} else {
		std::cerr << "[MISSING] populationMu value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("populationReEvaluationRate")) {
		convertFromString<double>(PopulationControlArchitecture::reEvaluationRate, gProperties.getProperty("populationReEvaluationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationReEvaluationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("populationCrossoverRate")) {
		convertFromString<double>(PopulationControlArchitecture::crossoverRate, gProperties.getProperty("populationCrossoverRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationCrossoverRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("populationMutationRate")) {
		convertFromString<double>(PopulationControlArchitecture::mutationRate, gProperties.getProperty("populationMutationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] populationMutationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	string fitnessCalculation;
	gProperties.checkAndGetPropertyValue("evolutionFitnessCalculation", &fitnessCalculation, true);
	if(fitnessCalculation == "average"){
		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
		EvolutionaryControlArchitecture::fitnessCalculation = EvolutionaryControlArchitecture::AVERAGE;
	}else if(fitnessCalculation == "cumulative"){
		std::cout << "fitness calculation: " << fitnessCalculation << std::endl;
		EvolutionaryControlArchitecture::fitnessCalculation = EvolutionaryControlArchitecture::CUMULATIVE;
	}
	
	gProperties.checkAndGetPropertyValue("gScaling", &RiverbedSharedData::SCALING, true);
	gProperties.checkAndGetPropertyValue("gMaxEnergy", &RiverbedSharedData::MAX_ENERGY, true);
	gProperties.checkAndGetPropertyValue("gStartEnergy", &RiverbedSharedData::START_ENERGY, true);
	
	gProperties.checkAndGetPropertyValue("gGenomesLogFilename", &RiverbedSharedData::gGenomesLogFilename, true);
	gProperties.checkAndGetPropertyValue("gOutputLogFilename", &RiverbedSharedData::gOutputLogFilename, true);
	
	cout << "Logging genomes to: " << RiverbedSharedData::gGenomesLogFilename << std::endl;
	cout << "Logging output to: " << RiverbedSharedData::gOutputLogFilename << std::endl;
	
	cout << "Scaling: " << RiverbedSharedData::SCALING << endl;
	cout << "Start Energy: " << RiverbedSharedData::START_ENERGY << endl;
	cout << "Max Energy: " << RiverbedSharedData::MAX_ENERGY << endl;
	
	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	
	return returnValue;
}