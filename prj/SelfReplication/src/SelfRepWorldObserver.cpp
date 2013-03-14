/*
 *  SelfRepWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Jean-Marc on 01/02/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Observers/AgentObserver.h"

#include "World/World.h"
#include "SelfReplication/include/SelfRepWorldObserver.h"
#include "Utilities/SDL_gfxPrimitives.h"

#include "SelfReplication/include/SelfRepAgentObserver.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "SelfReplication/include/SelfRepPerceptronControlArchitecture.h"


SelfRepWorldObserver::SelfRepWorldObserver( World *__world ) :
	WorldObserver( __world )
{
	_world = __world;
	gProperties.checkAndGetPropertyValue("sigmaRef", &_sigmaRef, true);
	gProperties.checkAndGetPropertyValue("iterationMax", &_iterationMax, true);
}

SelfRepWorldObserver::~SelfRepWorldObserver()
{
	// nothing to do.
}

void SelfRepWorldObserver::reset()
{
	// nothing to do.
}

void SelfRepWorldObserver::step()
{
	std::vector<RobotAgentPtr> tmpAgents;

	int unactiveEnergyPoint = 0;
	for (std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it < gEnergyPoints.end() ; it++ )
	{
		if ( ! it->getActiveStatus() )
		{
			unactiveEnergyPoint ++;
			it->step();
		}
	}
	gLogFile << gWorld->getIterations() << " :: Nb of active energy point: " << gEnergyPoints.size() - unactiveEnergyPoint << std::endl;

	for (int i = 0 ; i < gWorld->getNbOfAgent() ; i++ )
	{
/*
		SelfRepAgentWorldModel *selfRepWm = dynamic_cast<SelfRepAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel());
		if( ! selfRepWm)
		{
			std::cout << "Error from robot nb." << i << " (SelfRepWorldObserver) : the WorldModel of this robot isn't a SelfRepAgentWorldModel" << std::endl;
			exit(1);
		}

		SelfRepPerceptronControlArchitecture *currentControlArchitecture = dynamic_cast<SelfRepPerceptronControlArchitecture*>(gWorld->getAgent(i)->getBehavior());
		if( ! currentControlArchitecture)
		{
			std::cout << "Error from robot nb." << i << " (SelfRepWorldObserver) : the BehaviorControler of this robot isn't a SelfRepPerceptronControlArchitecture" << std::endl;
			exit(1);
		}

		if ( selfRepWm->getDuplicate())
		{
			int fatherX, fatherY =0;
			int sonX, sonY = 0;
			gWorld->getAgent(i)->getCoord(fatherX,fatherY);

			RobotAgentPtr son = boost::make_shared<RobotAgent>(_world);
			do
			{
				sonX = ( (fatherX ) + ((rand()%1000)/100)-5 );
			}while ((sonX < 10) || (sonX>gAreaWidth-10));

			do
			{
				sonY = ( (fatherY ) + ((rand()%1000)/100)-5 ) ;
			}while ((sonY < 10) || (sonY>gAreaHeight-10));

			son->setCoordReal(sonX,sonY);


			SelfRepAgentWorldModel *sonWorldModel = dynamic_cast<SelfRepAgentWorldModel*>(son->getWorldModel());
			if( ! sonWorldModel)
			{
				std::cout << "The WorldModel of the newly created robot nb." << son->getWorldModel()->_agentId << " isn't an SelfRepAgentWorldModel" << std::endl;
				exit(1);
			}

			std::vector<double> sonGenome = currentControlArchitecture->mutateWithBouncingBounds(_sigmaRef);
			sonWorldModel->_genome = sonGenome;
			gLogFile  << gWorld->getIterations() << " :: Robot nb." << sonWorldModel->getUniqueAgentId() << " #ReproductionRate: " << std::fixed << std::showpoint << (sonGenome[28]+4.0)/8.0 << std::endl;
			gLogFile  << gWorld->getIterations() << " :: Robot nb." << sonWorldModel->getUniqueAgentId() << " #TransmitEnergy: " << std::fixed << std::showpoint << (sonGenome[27]+4.0)/8.0 << std::endl;
			double transmittedEnergy = ((sonGenome[27]+4.0)/8.0)*0.1*_iterationMax;	
			if (transmittedEnergy > selfRepWm->getEnergy())
			{
				transmittedEnergy = selfRepWm->getEnergy();
			}
			sonWorldModel->setEnergy(transmittedEnergy);	
			gLogFile  << gWorld->getIterations() << " :: Robot nb." << sonWorldModel->getUniqueAgentId() << " #BirthEnergy: " << transmittedEnergy << std::endl;
			gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " has transmit energy " << transmittedEnergy << std::endl;
			selfRepWm->setEnergy(selfRepWm->getEnergy()-transmittedEnergy);	

			tmpAgents.push_back(son);
		}

	}

	bool variation = true;
	while (variation)
	{
		variation = false;
		for (int i = 0 ; i < gWorld->getNbOfAgent() ; i++ )
		{
			SelfRepAgentWorldModel *selfRepWm = dynamic_cast<SelfRepAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel());
			if ( ! selfRepWm->getStillAlive())
			{
				if (selfRepWm->getEnergy() <= 0.0 )
				{
					gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " die (starving) " <<  selfRepWm->getEnergy() << std::endl;
				}
				else
				{
					gLogFile  << gWorld->getIterations() << " :: Robot nb." << selfRepWm->getUniqueAgentId() << " die (oldness) " <<  selfRepWm->getEnergy() << std::endl;
				}
				gWorld->deleteAgent(i);
				variation = true;
			}
		}
	}

	for(unsigned int i=0 ; i < tmpAgents.size() ; i++ )
	{
		gWorld->addAgent(tmpAgents[i]);
	}

	gLogFile << gWorld->getIterations() << " :: Nb of robots: " << gWorld->getNbOfAgent() << std::endl;

	gEnergyPointRespawnLagMaxValue = pow((gWorld->getNbOfAgent()/2.0),1.2);
	*/
	}
}


