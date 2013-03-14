/*
 *  ReplayAgentObserver.cpp
 *  Roborobo
 *
 *  Created by Jean-Marc on 15/12/09.
 *
 */


#include "mEDEA/include/ReplayAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "BehaviorControlArchitectures/EnergyPerceptronControlArchitecture.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"


ReplayAgentObserver::ReplayAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (MedeaAgentWorldModel*)__wm;

		gProperties.checkAndGetPropertyValue("gEnergyMax",&MedeaSharedData::gEnergyMax,true);
		gProperties.checkAndGetPropertyValue("gEnergyInit",&MedeaSharedData::gEnergyInit,true);
		gProperties.checkAndGetPropertyValue("gEnergyRevive",&MedeaSharedData::gEnergyRevive,true);
	if ( gProperties.checkAndGetPropertyValue("gExperimentNumber",&MedeaSharedData::gExperimentNumber,false) == false )
	{
		if ( gVerbose )
			std::cout << "Medea property missing: MedeaSharedData::gExperimentNumber (default value assumed)." << std::endl;
		MedeaSharedData::gExperimentNumber = 0;
	}

	_iterationCount = 0 ;
	std::stringstream file;
	file << _wm->_agentId << ".dat";
	__logManager = new LogManager(file.str());
	__logManager->write("descrpition:x y");
	std::stringstream toWrite;
	toWrite << "start:" << gWorld->getIterations();
	__logManager->write(toWrite.str());
}

ReplayAgentObserver::~ReplayAgentObserver()
{
	// nothing to do.
}

void ReplayAgentObserver::reset()
{
	// nothing to do 
}

void ReplayAgentObserver::step()
{
	_iterationCount++;
	if ( _wm->_agentId == 0 )
	{
			if ( MedeaSharedData::gExperimentNumber == 2 )
			{
					if (_wm->_agentId ==  gAgentIndexFocus ) // && gVerbose ) // debug
					{
						std::cout << std::endl << "#### Experiment no.2 starts now. ####" << std::endl;
					}
				
					// * remove energy points.
				
					for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
					{
						it->hide();
					}
					gEnergyPoints.clear();
				
					// * setup new energy zone
				
					Uint32 colorShown = 0xeab71fff;
					Uint32 colorZone  = 0xAAAAAAFF; // for floor sensor.
				
					for (Sint16 xColor = MedeaSharedData::g_xStart_EnergyZone ; xColor < MedeaSharedData::g_xEnd_EnergyZone ; xColor++)
					{
						for (Sint16 yColor = Sint16 (MedeaSharedData::g_yStart_EnergyZone) ; yColor < Sint16 (MedeaSharedData::g_yEnd_EnergyZone) ; yColor ++)
						{
								pixelColor(gBackgroundImage, xColor, yColor, colorShown);
								pixelColor(gZoneImage, xColor, yColor, colorZone);
						}
					}
			
			}
	}
					
//	std::cout << "robot #" << _wm->_agentId << "\n" ;

	MedeaPerceptronControlArchitecture* currentBehavior = dynamic_cast<MedeaPerceptronControlArchitecture*>(gWorld->getAgent(_wm->_agentId)->getBehavior());

	if ( ! currentBehavior )
	{
		std::cerr << "Error from robot " << _wm->_agentId << " : the behavior architecture of this robot  isn't a MedeaPerceptronControlArchitecture" << std::endl;
		exit(1);
	}

	if ( MedeaSharedData::gExperimentNumber == 1 )
	{
		// * update the energy of the robot (if needed)
	
		Point2d posRobot(_wm->_xReal,_wm->_yReal);
		if ( gEnergyMode )
		{
			for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
			{

				if( (getEuclidianDistance(posRobot,it->getPosition()) < gEnergyPointRadius) && (it->getActiveStatus()))
				{
					float loadingEnergy = gEnergyPointValue; // test?

					// update energy level
					_wm->setEnergyLevel(_wm->getEnergyLevel() + loadingEnergy);
					_wm->setDeltaEnergy(_wm->getDeltaEnergy() + loadingEnergy);

					//saturate
					if ( _wm->getEnergyLevel() > MedeaSharedData::gEnergyMax ) // should be at least one lifetime
						_wm->setEnergyLevel(MedeaSharedData::gEnergyMax);
					if ( _wm->getDeltaEnergy() > MedeaSharedData::gEnergyMax )
						_wm->setDeltaEnergy(MedeaSharedData::gEnergyMax);

					gLogFile << "Info(" << gWorld->getIterations() << ") : " << _wm->_agentId
						<< "(" << posRobot.x << "," << posRobot.y << ")" 
						<< " get an energy point at " << it->getPosition().x << "," << it->getPosition().y << " :: Value : " << loadingEnergy  << std::endl;

					it->setActiveStatus(false);
				}
			}
		}
	}
	else 
	{
		if ( MedeaSharedData::gExperimentNumber == 2 )
		{
			// * once per world update (TODO: move to worldobserver)
			
			if (_wm->_agentId ==  0 ) // debug
			{
				int agentsOnZone = 0;
				for ( int i = 0 ; i != gAgentCounter ; i++ )
				{
					//int x = (int)(gWorld->getAgent(i)->getWorldModel()->getXReal());
					//int y = (int)(gWorld->getAgent(i)->getWorldModel()->getYReal());
					// std::cout << "x =" << x << " , y = " << y << std::endl;

					//if ( x >= MedeaSharedData::g_xStart_EnergyZone && y >= MedeaSharedData::g_yStart_EnergyZone && x <= MedeaSharedData::g_xEnd_EnergyZone && y <= MedeaSharedData::g_yEnd_EnergyZone )
					//	agentsOnZone++;
				}
				// update MedeaSharedData::gZoneEnergy_harvestValue
				//MedeaSharedData::gZoneEnergy_harvestValue = 10; // TODO :: TEMPORARY !!!!!!!!!!TEMPORARY !!!!!!!!!!TEMPORARY !!!!!!!!!!TEMPORARY !!!!!!!!!!TEMPORARY !!!!!!!!!!
				if ( gVerbose )
					std::cout << "There are " << agentsOnZone << " agents on the energy zone" <<  std::endl;
			
				/**/
				if ( agentsOnZone <= MedeaSharedData::gZoneEnergy_maxFullCapacity )
				{
					// best case
					MedeaSharedData::gZoneEnergy_harvestValue = MedeaSharedData::gZoneEnergy_maxHarvestValue;
				}
				else
				{
					if ( agentsOnZone <= MedeaSharedData::gZoneEnergy_saturateCapacityLevel )
					{
						double energyValueSpan = MedeaSharedData::gZoneEnergy_maxHarvestValue - MedeaSharedData::gZoneEnergy_minHarvestValue;
						int agentsOverheadCount = MedeaSharedData::gZoneEnergy_saturateCapacityLevel - MedeaSharedData::gZoneEnergy_maxFullCapacity;
						double costPerAgents = energyValueSpan / (double)agentsOverheadCount;
						MedeaSharedData::gZoneEnergy_harvestValue = MedeaSharedData::gZoneEnergy_maxHarvestValue - costPerAgents * ( agentsOnZone- MedeaSharedData::gZoneEnergy_maxFullCapacity ) ;
					}
					else 
					{
						// worst case
						MedeaSharedData::gZoneEnergy_harvestValue = MedeaSharedData::gZoneEnergy_minHarvestValue;
					}
				}
				/**/
				
				// debug : MedeaSharedData::gZoneEnergy_harvestValue = MedeaSharedData::gZoneEnergy_maxHarvestValue;
			
			}

		
			// * for each agent -- TODO: could be optimized by merging with previous block in the worldobserve

			if ( _wm->_xReal >= MedeaSharedData::g_xStart_EnergyZone && _wm->_xReal <= MedeaSharedData::g_xEnd_EnergyZone && _wm->_yReal >= MedeaSharedData::g_yStart_EnergyZone && _wm->_yReal <= MedeaSharedData::g_yEnd_EnergyZone )
			{
				float loadingEnergy = MedeaSharedData::gZoneEnergy_harvestValue;
				
				// update energy level
				_wm->setEnergyLevel(_wm->getEnergyLevel() + loadingEnergy);
				_wm->setDeltaEnergy(_wm->getDeltaEnergy() + loadingEnergy);

				// saturate
				if ( _wm->getEnergyLevel() > MedeaSharedData::gEnergyMax ) // assume: need MedeaSharedData::gEvaluationTime to live full life
					_wm->setEnergyLevel(MedeaSharedData::gEnergyMax);
				if ( _wm->getDeltaEnergy() > MedeaSharedData::gEnergyMax ) // assume: need MedeaSharedData::gEvaluationTime to live full life 
					_wm->setDeltaEnergy(MedeaSharedData::gEnergyMax);

				Point2d posRobot(_wm->_xReal,_wm->_yReal);				
				gLogFile << "Info(" << gWorld->getIterations() << ") : " << _wm->_agentId
						<< "(" << posRobot.x << "," << posRobot.y << ")" 
						<< " get an energy point at 0,0 :: Value : " << loadingEnergy  << std::endl; // hack to comply with python log analyser
			}
		}
	}

	// * check energy level

	if ( MedeaSharedData::gExperimentNumber == 1 || MedeaSharedData::gExperimentNumber == 2 )
	{
		if ( _wm->getEnergyLevel() <= 0 )
		{
			_wm->setDeltaEnergy(0); // must be set to zero to avoid broadcasting.
			_wm->setActiveStatus(false);
		}
	}
	
	std::stringstream logMsg ;
	logMsg << _wm->_xReal << " " << _wm->_yReal << " " << _wm->getEnergyLevel();
	__logManager->write(logMsg.str());
}

void ReplayAgentObserver::loadGenome(std::vector<double> inGenome)
{
	std::cout << std::flush ;
	_wm->_genome.clear();
	for ( unsigned int i = 0 ; i != inGenome.size() ; i++ )
	{
		_wm->_genome.push_back(inGenome[i]);
	}
	_currentGenome = _wm->_genome;
	_wm->setNewGenomeStatus(true);
//	MedeaPerceptronControlArchitecture* currentBehavior = dynamic_cast<MedeaPerceptronControlArchitecture*>(gWorld->getAgent(_wm->_agentId)->getBehavior());
	_wm->setDeltaEnergy(10.0);
}

void ReplayAgentObserver::logStatus()
{
	MedeaPerceptronControlArchitecture* currentBehavior = dynamic_cast<MedeaPerceptronControlArchitecture*>(gWorld->getAgent(_wm->_agentId)->getBehavior());
	if ( ! currentBehavior )
	{
		std::cerr << "Error from robot " << _wm->_agentId << " : the behavior architecture of this robot  isn't a EnergyPerceptronControlArchitecture" << std::endl;
		exit(1);
	}

	//Display the current state of the controller
	gLogFile  << "#GIteration: " << gWorld->getIterations() << " #Robot: " << _wm->_agentId
		<< " #Energy: " <<  _wm->getEnergyLevel()
		<< " #DeltaEnergy: " << _wm->getDeltaEnergy() << std::endl;
}
