/*
 *  SelfRepAgentObserver.cpp
 *  Roborobo
 *
 *  Created by Jean-Marc on 04/03/10.
 *
 */


#include "SelfReplication/include/SelfRepAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "SelfReplication/include/SelfRepPerceptronControlArchitecture.h"
#include <cmath>


SelfRepAgentObserver::SelfRepAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = __wm;
	SelfRepAgentWorldModel* selfRepWm = dynamic_cast<SelfRepAgentWorldModel*>(_wm);
	if ( ! selfRepWm )
	{
		std::cerr << "Error from robot " << _wm->_agentId << " : the behavior architecture of this robot  isn't a SelfRepPerceptronControlArchitecture" << std::endl;
		exit(1);
	}
	std::stringstream file;
	file << selfRepWm->getUniqueAgentId() << ".dat";
	__logManager = new LogManager(file.str());
	__logManager->write("descrpition:x y");
	std::stringstream toWrite;
	toWrite << "start:" << gWorld->getIterations();
	__logManager->write(toWrite.str());
}

SelfRepAgentObserver::~SelfRepAgentObserver()
{
	// nothing to do.
}

void SelfRepAgentObserver::reset()
{
	// nothing to do 
}

void SelfRepAgentObserver::step()
{
	SelfRepAgentWorldModel* selfRepWm = dynamic_cast<SelfRepAgentWorldModel*>(_wm);
	if ( ! selfRepWm )
	{
		std::cerr << "Error from robot " << _wm->_agentId << " : the behavior architecture of this robot  isn't a SelfRepPerceptronControlArchitecture" << std::endl;
		exit(1);
	}

	//Display the current state of the controller
	gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " #Energy: " <<  selfRepWm->getEnergy() << std::endl;
	gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " #Replication: "  << selfRepWm->getDuplicate()<< std::endl;
	gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " #Age: " << selfRepWm->getAge()<< std::endl;
	gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " #Key: " << std::fixed << std::showpoint << selfRepWm->getKey() << std::endl;
	gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " #Genome: ";
	for (unsigned int i=0 ; i < selfRepWm->_genome.size() ; i++ )
	{
		gLogFile << std::fixed << std::showpoint << selfRepWm->_genome[i] << " " ;
	}
	gLogFile << std::endl;
	std::stringstream logMsg;
	logMsg << selfRepWm->getXReal() << " " << selfRepWm->getYReal();
	__logManager->write(logMsg.str());
}
