/*
 *  NewscastConfigurationLoader.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Config/NewscastConfigurationLoader.h"

#include "IPA-Newscast/include/NewscastWorldObserver.h"
#include "IPA-Newscast/include/NewscastAgentWorldModel.h"
#include "IPA-Newscast/include/NewscastAgentObserver.h"
#include "IPA-Newscast/include/NewscastControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

NewscastConfigurationLoader::NewscastConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

NewscastConfigurationLoader::~NewscastConfigurationLoader()
{
	delete _AgentWorldInterface;
}


WorldObserver* NewscastConfigurationLoader::make_WorldObserver(World* wm)
{
	return new NewscastWorldObserver(wm);
}

RobotAgentWorldModel* NewscastConfigurationLoader::make_RobotAgentWorldModel()
{
	return new NewscastAgentWorldModel();
}

AgentObserver* NewscastConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new NewscastAgentObserver(wm);
}

BehaviorControlArchitecture* NewscastConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new NewscastControlArchitecture(wm);
}

RobotAgentWorldInterface* NewscastConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool NewscastConfigurationLoader::readProperties(ExtendedProperties *gProperties)
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
	
	if (gProperties->hasProperty("newscastCacheSize")) {
		convertFromString<int>(NewscastControlArchitecture::cacheSize, gProperties->getProperty("newscastCacheSize"), std::dec);
	} else {
		std::cerr << "[MISSING] newscastCacheSize value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("newscastItemTTL")) {
		convertFromString<int>(NewscastControlArchitecture::itemTTL, gProperties->getProperty("newscastItemTTL"), std::dec);
	} else {
		std::cerr << "[MISSING] newscastItemTTL value is missing." << std::endl;
		returnValue = false;
	}
        
        if (gProperties->hasProperty("populationNumberOfParents")) {
		convertFromString<int>(PopulationControlArchitecture::numberOfParents, gProperties->getProperty("populationNumberOfParents"), std::dec);
	} else {
		std::cerr << "[MISSING] populationNumberOfParents value is missing." << std::endl;
		returnValue = false;
	}
	
	std::cout << "steps per candidate: " << EvolutionaryControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "mu: " << PopulationControlArchitecture::mu << std::endl;
	std::cout << "re-evaluation rate: " << PopulationControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << PopulationControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << PopulationControlArchitecture::mutationRate << std::endl;
	std::cout << "cache size: " << NewscastControlArchitecture::cacheSize << std::endl;
	std::cout << "item TTL: " << NewscastControlArchitecture::itemTTL << std::endl;
        std::cout << "Number Of Parents: " << NewscastControlArchitecture::numberOfParents << std::endl;
	
	return returnValue;
}
