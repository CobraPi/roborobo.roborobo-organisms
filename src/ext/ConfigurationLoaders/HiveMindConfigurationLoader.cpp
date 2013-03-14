/*
 *  HiveMindConfigurationLoader.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "Config/HiveMindConfigurationLoader.h"

#include "IPA-HiveMind/include/HiveMindWorldObserver.h"
#include "IPA-HiveMind/include/HiveMindAgentWorldModel.h"
#include "IPA-HiveMind/include/HiveMindAgentObserver.h"
#include "IPA-HiveMind/include/HiveMindControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

HiveMindConfigurationLoader::HiveMindConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

HiveMindConfigurationLoader::~HiveMindConfigurationLoader()
{
	//nothing to do
}


WorldObserver* HiveMindConfigurationLoader::make_WorldObserver(World* wm)
{
	return new HiveMindWorldObserver(wm);
}

RobotAgentWorldModel* HiveMindConfigurationLoader::make_RobotAgentWorldModel()
{
	return new HiveMindAgentWorldModel();
}

AgentObserver* HiveMindConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new HiveMindAgentObserver(wm);
}

BehaviorControlArchitecture* HiveMindConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new HiveMindControlArchitecture(wm);
}

RobotAgentWorldInterface* HiveMindConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool HiveMindConfigurationLoader::readProperties(ExtendedProperties *gProperties)
{
	bool returnValue = true;
	
	std::cout << "Reading properties..." << std::endl;
		
	if (gProperties->hasProperty("evolutionStepsPerCandidate")) {
		convertFromString<int>(EvolutionaryControlArchitecture::stepsPerCandidate, gProperties->getProperty("evolutionStepsPerCandidate"), std::dec);
	} else {
		std::cerr << "[MISSING] evolutionStepsPerCandidate value is missing." << std::endl;
		returnValue = false;
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
	
	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	
	return returnValue;
}