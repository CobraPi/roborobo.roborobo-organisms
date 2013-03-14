#include "Config/ChangingWorldConfigurationLoader.h"

#include "ChangingWorld/include/ChangingWorldWorldObserver.h"
#include "ChangingWorld/include/ChangingWorldAgentWorldModel.h"
#include "ChangingWorld/include/ChangingWorldAgentObserver.h"
#include "ChangingWorld/include/ChangingWorldControlArchitecture.h"
#include "ChangingWorld/include/ChangingWorldSharedData.h"
#include "ChangingWorld/include/ChangingWorldWorldInterface.h"

#include "World/RobotAgentWorldInterface.h"

#include <boost/algorithm/string.hpp>

ChangingWorldConfigurationLoader::ChangingWorldConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new ChangingWorldWorldInterface();
	readProperties(&gProperties);
}

ChangingWorldConfigurationLoader::~ChangingWorldConfigurationLoader()
{
	//nothing to do
	delete _AgentWorldInterface;
}


WorldObserver* ChangingWorldConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ChangingWorldWorldObserver(wm);
}

RobotAgentWorldModel* ChangingWorldConfigurationLoader::make_RobotAgentWorldModel()
{
	return new ChangingWorldAgentWorldModel();
}

AgentObserver* ChangingWorldConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ChangingWorldAgentObserver(wm);
}

BehaviorControlArchitecture* ChangingWorldConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new ChangingWorldControlArchitecture(wm);
}

RobotAgentWorldInterface* ChangingWorldConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool ChangingWorldConfigurationLoader::readProperties(ExtendedProperties *gProperties)
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
	
	if (gProperties->hasProperty("populationNumberOfParents")) {
		convertFromString<int>(PopulationControlArchitecture::numberOfParents, gProperties->getProperty("populationNumberOfParents"), std::dec);
	} else {
		std::cerr << "[MISSING] populationNumberOfParents value is missing." << std::endl;
		returnValue = false;
	}
	gProperties->checkAndGetPropertyValue("gOutputLogFilename", &ChangingWorldSharedData::gOutputLogFilename, true);
	gProperties->checkAndGetPropertyValue("gLocationLogFilename", &ChangingWorldSharedData::gLocationLogFilename, true);
    gProperties->checkAndGetPropertyValue("factorLifetime", &ChangingWorldSharedData::factorLifetime , true);
	gProperties->checkAndGetPropertyValue("gMaxEnergy", &ChangingWorldSharedData::maxEnergy, true);
	gProperties->checkAndGetPropertyValue("gStartEnergy", &ChangingWorldSharedData::startEnergy, true);
	
    
    if (gProperties->hasProperty("gScaleFactor")) {
        std::string factor = gProperties->getProperty("gScaleFactor");
        std::vector<std::string> strs;
        boost::split(strs,factor,boost::is_any_of(","));
        
        for(int i=0;i<strs.size();i++){
            int num;
            convertFromString<int>(num,strs[i],std::dec);
            ChangingWorldSharedData::scaleFactor.push_back(num);
        }
	} else {
		std::cerr << "[MISSING] populationNumberOfParents value is missing." << std::endl;
		returnValue = false;
	}

	return returnValue;
}
