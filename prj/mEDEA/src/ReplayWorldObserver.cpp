/*
 *  ReplayWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Jean-Marc on 20/04/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Observers/AgentObserver.h"

#include "World/World.h"
#include "mEDEA/include/ReplayWorldObserver.h"

#include "mEDEA/include/ReplayAgentObserver.h"

#include "Utilities/LogManager.h"

ReplayWorldObserver::ReplayWorldObserver( World *__world ) :
	WorldObserver( __world )
{
	_world = __world;
	_firstStep = true;
	loadGenome();
	LogManager::set_path_d("./logs/");
}

ReplayWorldObserver::~ReplayWorldObserver()
{
	// nothing to do.
}

void ReplayWorldObserver::reset()
{
	// nothing to do.
}

void ReplayWorldObserver::step()
{
	if (_firstStep)
	{/*
		ReplayAgentObserver* agentObserver = dynamic_cast<ReplayAgentObserver*>(gWorld->getAgent(0)->getObserver());
		if ( ! agentObserver)
		{
			std::cerr << " Error from ReplayWorldObserver : the observer of robot 0 isn't a BatlleAgentObserver " << std ::endl;
			exit(1);
		}
		agentObserver->loadGenome(_genomeToReplay);
		_firstStep = false ;
		*/
	}

	for (std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it < gEnergyPoints.end(); it++)
	{
		if ( ! it->getActiveStatus() )
		{
			it->step();
		}
	}

}

void ReplayWorldObserver::loadGenome()
{
	int nbHiddenNeurons;
	gProperties.checkAndGetPropertyValue("nbHiddenNeurons",&nbHiddenNeurons,true);
	int nbInput = 8 + 5 ; //sensors + floorSensor + energyDirection + energyLevel + energyDirection + bias
	int nbOutput = 2 ; // rotation + velocity 
	int nbGene = ( nbInput * nbHiddenNeurons ) + ( nbHiddenNeurons * nbOutput) + nbOutput ; // from input to hidden layer + from hiddenLayer to output + from bias to output
	for (int i=0 ; i< nbGene ; i++)
	{
		double gene = 0;
		std::stringstream out;
		std::string s = "";
		s += "genomeToReplay[";
		out<<i;
		s+=out.str();
		s +="]";

		if ( gProperties.hasProperty ( s ) )
			convertFromString<double>(gene, gProperties.getProperty( s ), std::dec);
		else
		{
			std::cout << "[error] ReplayWorldObserver :: The gene " << i << " is missing from genomeToRead "<< std::endl;
			exit(1);
		}
		_genomeToReplay.push_back(gene);
	}
}

