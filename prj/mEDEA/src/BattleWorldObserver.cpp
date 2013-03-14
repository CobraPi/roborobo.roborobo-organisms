/*
 *  BattleWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Jean-Marc on 01/02/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Observers/AgentObserver.h"

#include "World/World.h"
#include "mEDEA/include/BattleWorldObserver.h"

#include "mEDEA/include/BattleAgentObserver.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"

BattleWorldObserver::BattleWorldObserver( World *__world ) :
	WorldObserver( __world )
{
	_world = __world;
	_energyPointUsed = 0;
	_firstStep = true;
	_nbGenomeToLoad = 10 ;
		gProperties.checkAndGetPropertyValue("nbGenomeToLoad",&_nbGenomeToLoad,true);
	loadGenomes();
}

BattleWorldObserver::~BattleWorldObserver()
{
	// nothing to do.
}

void BattleWorldObserver::reset()
{
	// nothing to do.
}

void BattleWorldObserver::incEnergyPointUsed()
{
	_energyPointUsed ++;
}

void BattleWorldObserver::step()
{
	if (_firstStep)
	{
		//Load the genomes in the bigger number of robots
		int agentPerGenome = gNbOfAgents/_nbGenomeToLoad ;
		for (int idGenome = 0; idGenome < _nbGenomeToLoad ; idGenome ++)
		{
			for (int idBot = agentPerGenome*idGenome ; idBot < agentPerGenome*(idGenome+1) ; idBot ++)
			{
				//BattleAgentObserver* agentObserver = dynamic_cast<BattleAgentObserver*>(gWorld->getAgent(idBot)->getObserver());
				//if ( ! agentObserver)
				//{
				//	std::cerr << " Error from BattleWorldObserver : the observer of robot " << idBot  << " isn't a BatlleAgentObserver " << std ::endl;
				//	exit(1);
				//}
				//agentObserver->loadGenome(_genomesToLoad[idGenome]);
			}
		}
		//desactivate the others
		for (int idBot = _nbGenomeToLoad*agentPerGenome ; idBot < gNbOfAgents ; idBot ++)
		{
			//MedeaPerceptronControlArchitecture* currentBehavior = dynamic_cast<MedeaPerceptronControlArchitecture*>(gWorld->getAgent(idBot)->getBehavior());
			//(dynamic_cast<MedeaAgentWorldModel*>(gWorld->getAgent(idBot)->getWorldModel()))->setActiveStatus(false);
		}
		_firstStep = false ;
	}

	for (std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it < gEnergyPoints.end(); it++)
	{
		if ( ! it->getActiveStatus() )
		{
			it->step();
		}
	}

}

void BattleWorldObserver::loadGenomes()
{
	for ( int toLoad = 0; toLoad < _nbGenomeToLoad ; toLoad ++)
	{
		std::vector<double> tmpGenome;
		int nbHiddenNeurons;
		gProperties.checkAndGetPropertyValue("nbHiddenNeurons",&nbHiddenNeurons,true);
		int nbInput = 8 + 5 ; //sensors + floorSensor + energyDirection + energyLevel + energyDirection + bias
		int nbOutput = 2 ; // rotation + velocity 
		int nbGene = ( nbInput * nbHiddenNeurons ) + ( nbHiddenNeurons * nbOutput) + nbOutput ; // from input to hidden layer + from hiddenLayer to output + from bias to output
		for (int i=0 ; i< nbGene ; i++)
		{
			double gene = 0;
			std::stringstream out;
			std::stringstream out2;
			std::string s = "";
			s += "genomeToLoad";
			out << toLoad;
			s += out.str();
			s+= "[";
			out2<<i;
			s+=out2.str();
			s +="]";

			if ( gProperties.hasProperty ( s ) )
				convertFromString<double>(gene, gProperties.getProperty( s ), std::dec);
			else
			{
				std::cout << "[error] BattleWorldObserver :: The gene " << i << " is missing from genomeToLoad " << toLoad << std::endl;
				exit(1);
			}
			tmpGenome.push_back(gene);
		}
		_genomesToLoad.push_back(tmpGenome);
	}
}

