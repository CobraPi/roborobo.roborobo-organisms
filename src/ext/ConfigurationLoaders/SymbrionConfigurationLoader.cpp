#include "Config/SymbrionConfigurationLoader.h"

#include "Symbrion/include/SymbrionWorldObserver.h"
#include "Symbrion/include/SymbrionAgentWorldModel.h"
#include "Symbrion/include/SymbrionAgentObserver.h"
#include "Symbrion/include/SymbrionControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"
#include "Symbrion/include/SymbrionSharedData.h"

SymbrionConfigurationLoader::SymbrionConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
	readProperties();
}

SymbrionConfigurationLoader::~SymbrionConfigurationLoader()
{
	//nothing to do
}


WorldObserver* SymbrionConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SymbrionWorldObserver(wm);
}

RobotAgentWorldModel* SymbrionConfigurationLoader::make_RobotAgentWorldModel()
{
	return new SymbrionAgentWorldModel();
}

AgentObserver* SymbrionConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new SymbrionAgentObserver(wm);
}

BehaviorControlArchitecture* SymbrionConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new SymbrionControlArchitecture(wm);
}

RobotAgentWorldInterface* SymbrionConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

bool SymbrionConfigurationLoader::readProperties()
{
	bool returnValue = true;
	
	std::cout << "Reading properties..." << std::endl;
		
	gProperties.checkAndGetPropertyValue("gEggAgentMaskImageFilename", &SymbrionSharedData::gEggAgentMaskImageFilename, true);
	gProperties.checkAndGetPropertyValue("gSeedAgentMaskImageFilename", &SymbrionSharedData::gSeedAgentMaskImageFilename, true);
	gProperties.checkAndGetPropertyValue("gFreeRiderAgentMaskImageFilename", &SymbrionSharedData::gFreeRiderAgentMaskImageFilename, true);
	
	if (gProperties.hasProperty("connectionType")) {
		convertFromString<int>(SymbrionSharedData::connectionType, gProperties.getProperty("connectionType"), std::dec);
		
		if( SymbrionSharedData::connectionType < 0 || SymbrionSharedData::connectionType > 9 ) {
			std::cerr << "[INVALID] connectionType must be between 0 and 9." << std::endl;
			returnValue = false;
		}
	} else {
		std::cerr << "[MISSING] connectionType." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxEggLifetime")) {
		convertFromString<int>(SymbrionSharedData::maxEggLifetime, gProperties.getProperty("maxEggLifetime"), std::dec);
	} else {
		std::cerr << "[MISSING] maxEggLifetime value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxSeedLifetime")) {
		convertFromString<int>(SymbrionSharedData::maxSeedLifetime, gProperties.getProperty("maxSeedLifetime"), std::dec);
	} else {
		std::cerr << "[MISSING] maxSeedLifetime value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxOrganismLifetime")) {
		convertFromString<int>(SymbrionSharedData::maxOrganismLifetime, gProperties.getProperty("maxOrganismLifetime"), std::dec);
	} else {
		std::cerr << "[MISSING] maxOrganismLifetime value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxSeedBoardcastRange")) {
		convertFromString<int>(SymbrionSharedData::maxSeedBoardcastRange, gProperties.getProperty("maxSeedBoardcastRange"), std::dec);
	} else {
		std::cerr << "[MISSING] maxSeedBoardcastRange value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxOrganismBoardcastRange")) {
		convertFromString<int>(SymbrionSharedData::maxOrganismBoardcastRange, gProperties.getProperty("maxOrganismBoardcastRange"), std::dec);
	} else {
		std::cerr << "[MISSING] maxOrganismBoardcastRange value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("maxOrganismSize")) {
		convertFromString<int>(SymbrionSharedData::maxOrganismSize, gProperties.getProperty("maxOrganismSize"), std::dec);
	} else {
		std::cerr << "[MISSING] maxOrganismSize value is missing." << std::endl;
		returnValue = false;
	}
	
	if (gProperties.hasProperty("minOrganismSize")) {
		convertFromString<int>(SymbrionSharedData::minOrganismSize, gProperties.getProperty("minOrganismSize"), std::dec);
	} else {
		std::cerr << "[MISSING] minOrganismSize value is missing." << std::endl;
		returnValue = false;
	}
	
	gProperties.checkAndGetPropertyValue("gGenomesOrgLogFilename", &SymbrionSharedData::gGenomesOrgLogFilename, true);
	SymbrionSharedData::gGenomesOrgLogFile.open(SymbrionSharedData::gGenomesOrgLogFilename.c_str());
	if (!SymbrionSharedData::gGenomesOrgLogFile) {
		std::cout << "[error] Cannot open log file " << SymbrionSharedData::gGenomesOrgLogFilename << "." << std::endl << std::endl;
	}
	
	SymbrionSharedData::gGenomesOrgLogFile << "AgentId, GenomeId, Parent1Id, Parent2Id, Size, NumberOfExtermities, NumberOfJointsGenome..." << std::endl;
	
	gProperties.checkAndGetPropertyValue("gEventLogFilename", &SymbrionSharedData::gEventLogFilename, true);
	SymbrionSharedData::gEventLogFile.open(SymbrionSharedData::gEventLogFilename.c_str());
	if (!SymbrionSharedData::gEventLogFile) {
		std::cout << "[error] Cannot open log file " << SymbrionSharedData::gEventLogFilename << "." << std::endl << std::endl;
	}
	
	SymbrionSharedData::gEventLogFile << "Iteration, Genome, EventNr, x, y, Time, Event" << std::endl;
	
	gProperties.checkAndGetPropertyValue("gTravelLogFilename", &SymbrionSharedData::gTravelLogFilename, true);
	SymbrionSharedData::gTravelLogFile.open(SymbrionSharedData::gTravelLogFilename.c_str());
	if (!SymbrionSharedData::gTravelLogFile) {
		std::cout << "[error] Cannot open log file " << SymbrionSharedData::gTravelLogFilename << "." << std::endl << std::endl;
	}
	
	SymbrionSharedData::gTravelLogFile << "Iteration, Genome, Distance Travelled" << std::endl;
	
	gProperties.checkAndGetPropertyValue("gPopLogFilename", &SymbrionSharedData::gPopLogFilename, true);
	SymbrionSharedData::gPopLogFile.open(SymbrionSharedData::gPopLogFilename.c_str());
	if (!SymbrionSharedData::gPopLogFile) {
		std::cout << "[error] Cannot open log file " << SymbrionSharedData::gPopLogFilename << "." << std::endl << std::endl;
	}
	
	SymbrionSharedData::gPopLogFile << "Iteration, Total Size, Eggs, Seeds, Adults, Free Riders, Abortions" << std::endl;
	
	gProperties.checkAndGetPropertyValue("gOutputLogFilename", &SymbrionSharedData::gOutputLogFilename, true);
    gProperties.checkAndGetPropertyValue("gHeatmapLogFilename", &SymbrionSharedData::gHeatmapLogFilename, true);    
	
    std::cout << "Logging output to: " << SymbrionSharedData::gOutputLogFilename << std::endl;
	
	return returnValue;
}