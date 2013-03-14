#include "Config/MPOConfigurationLoader.h"

#include "IPA-MuPlusOne/include/MPOWorldObserver.h"
#include "IPA-MuPlusOne/include/MPOAgentWorldModel.h"
#include "IPA-MuPlusOne/include/MPOAgentObserver.h"
#include "IPA-MuPlusOne/include/MPOControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

MPOConfigurationLoader::MPOConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties(&gProperties);
}

MPOConfigurationLoader::~MPOConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MPOConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MPOWorldObserver(wm);
}

RobotAgentWorldModel* MPOConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MPOAgentWorldModel();
}

AgentObserver* MPOConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MPOAgentObserver(wm);
}

BehaviorControlArchitecture* MPOConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MPOControlArchitecture(wm);
}

RobotAgentWorldInterface* MPOConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool MPOConfigurationLoader::readProperties(ExtendedProperties *gProperties)
{
	bool returnValue = true;
	
	std::cout << "Reading properties..." << std::endl;
	
	if (gProperties->hasProperty("mpoMu")) {
		convertFromString<unsigned int>(MPOControlArchitecture::mu, gProperties->getProperty("mpoMu"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoMu value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("mpoStepsPerCandidate")) {
		convertFromString<int>(MPOControlArchitecture::stepsPerCandidate, gProperties->getProperty("mpoStepsPerCandidate"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoStepsPerCandidate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("mpoReEvaluationRate")) {
		convertFromString<double>(MPOControlArchitecture::reEvaluationRate, gProperties->getProperty("mpoReEvaluationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoReEvaluationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("mpoCrossoverRate")) {
		convertFromString<double>(MPOControlArchitecture::crossoverRate, gProperties->getProperty("mpoCrossoverRate"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoCrossoverRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("mpoMutationRate")) {
		convertFromString<double>(MPOControlArchitecture::mutationRate, gProperties->getProperty("mpoMutationRate"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoMutationRate value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties->hasProperty("mpoInitialMutationStepSize")) {
		convertFromString<double>(Controller::initialMutationStepSize, gProperties->getProperty("mpoInitialMutationStepSize"), std::dec);
	} else {
		std::cerr << "[MISSING] mpoInitialMutationStepSize value is missing." << std::endl;
		returnValue = false;
	}
        
        if (gProperties->hasProperty("populationNumberOfParents")) {
		convertFromString<int>(MPOControlArchitecture::numberOfParents, gProperties->getProperty("populationNumberOfParents"), std::dec);
	} else {
		std::cerr << "[MISSING] populationNumberOfParents value is missing." << std::endl;
		returnValue = false;
	}

	std::cout << "mu: " << MPOControlArchitecture::mu << std::endl;
	std::cout << "steps per candidate: " << MPOControlArchitecture::stepsPerCandidate << std::endl;
	std::cout << "re-evaluation rate: " << MPOControlArchitecture::reEvaluationRate << std::endl;
	std::cout << "crossover rate: " << MPOControlArchitecture::crossoverRate << std::endl;
	std::cout << "mutation rate: " << MPOControlArchitecture::mutationRate << std::endl;
	std::cout << "initial mutation step size: " << Controller::initialMutationStepSize << std::endl;
	std::cout << "populationNumberOfParents: " << MPOControlArchitecture::numberOfParents << std::endl;
        
	return returnValue;
}