/*
 *  BattleAgentObserver.cpp
 *  Roborobo
 *
 *  Created by Jean-Marc on 15/12/09.
 *
 */


#include "mEDEA/include/BattleAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "BehaviorControlArchitectures/EnergyPerceptronControlArchitecture.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"


BattleAgentObserver::BattleAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (MedeaAgentWorldModel*)__wm;
/*
		gProperties.checkAndGetPropertyValue("g_xStart_EnergyZone",&MedeaSharedData::g_xStart_EnergyZone,true);
		gProperties.checkAndGetPropertyValue("g_yStart_EnergyZone",&MedeaSharedData::g_yStart_EnergyZone,true);
		gProperties.checkAndGetPropertyValue("g_xEnd_EnergyZone",&MedeaSharedData::g_xEnd_EnergyZone,true);
		gProperties.checkAndGetPropertyValue("g_yEnd_EnergyZone",&MedeaSharedData::g_yEnd_EnergyZone,true);
	
		gProperties.checkAndGetPropertyValue("gZoneEnergy_maxHarvestValue",&MedeaSharedData::gZoneEnergy_maxHarvestValue,true);
		gProperties.checkAndGetPropertyValue("gZoneEnergy_minHarvestValue",&MedeaSharedData::gZoneEnergy_minHarvestValue,true);
		gProperties.checkAndGetPropertyValue("gZoneEnergy_maxFullCapacity",&MedeaSharedData::gZoneEnergy_maxFullCapacity,true);
		gProperties.checkAndGetPropertyValue("gZoneEnergy_saturateCapacityLevel",&MedeaSharedData::gZoneEnergy_saturateCapacityLevel,true);
		
		gProperties.checkAndGetPropertyValue("gMaxPenalizationRate",&MedeaSharedData::gMaxPenalizationRate,true);
		if ( MedeaSharedData::gMaxPenalizationRate < 0 || MedeaSharedData::gMaxPenalizationRate > 1 )
		{
			std::cerr << "gMaxPenalizationRate should be defined btw O and 1" << std::endl;
			exit(-1);
		}*/

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}

	gProperties.checkAndGetPropertyValue("synchronization",&_synchronization,true);
		gProperties.checkAndGetPropertyValue("gEnergyMax",&MedeaSharedData::gEnergyMax,true);
		gProperties.checkAndGetPropertyValue("gEnergyInit",&MedeaSharedData::gEnergyInit,true);
		gProperties.checkAndGetPropertyValue("gEnergyRevive",&MedeaSharedData::gEnergyRevive,true);

	if ( gProperties.checkAndGetPropertyValue("gExperimentNumber",&MedeaSharedData::gExperimentNumber,false) == false )
	{
		if ( gVerbose )
			std::cout << "Medea property missing: MedeaSharedData::gExperimentNumber (default value assumed)." << std::endl;
		MedeaSharedData::gExperimentNumber = 0;
	}
		gProperties.checkAndGetPropertyValue("gEvaluationTime",&MedeaSharedData::gEvaluationTime,true);
	_minValue = -1.0;
	_maxValue = 1.0;

	_iterationCount = 0;
	_evaluationCount = 0;
	_side = -1;

	//resetActiveGenome();
}

BattleAgentObserver::~BattleAgentObserver()
{
	// nothing to do.
}

void BattleAgentObserver::reset()
{
	// nothing to do 
}

void BattleAgentObserver::step()
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

	/*if (_wm->_agentId == 1)
	{
		std::cout << _key <<std::endl;
	}*/
	if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
	{
		//std::cout << "target: " << _wm->getEnergyPointDirectionAngleValue() << std::endl;// " (" << _wm->_agentAbsoluteOrientation << "," << angleToClosestEnergyPoint << ")" << std::endl;
	}

	if ( MedeaSharedData::gExperimentNumber == 1 )
	{
		// * update the energy of the robot (if needed)
	
		Point2d posRobot(_wm->_xReal,_wm->_yReal);
		if ( gEnergyMode )
		{
			for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
			{

				if( (getEuclidianDistance (posRobot,it->getPosition()) < gEnergyPointRadius) && (it->getActiveStatus()))
				{
					float loadingEnergy = gEnergyPointValue; // test?
					//float loadingEnergy = 5*(1.0/(2.0*sqrt(2.0*M_PI)))*gEnergyPointValue; // test?
					//float loadingEnergy = 5*(1.0/(2.0*sqrt(2.0*M_PI)))*exp(-(pow((_key - it->getKey()),2.0)/(pow(2.0,2.0))))*gEnergyPointValue;

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
					///	agentsOnZone++;
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

	// * broadcast the genome (current agent writes its genome to all neighbors
		
	// broadcast only if agent is active (ie. not just revived) and deltaE>0.
	if ( 
			/*( _wm->getDeltaEnergy()>0.0 && _wm->getActiveStatus() == true )
			||
			( MedeaSharedData::gExperimentNumber == 0 && _wm->getActiveStatus() == true )*/
			_wm->getActiveStatus() == true
		)  
	{
		for (int i = 0 ; i < gAgentCounter ; i++)
		{
			if ( ( i != _wm->_agentId ) && ( gRadioNetworkArray[_wm->_agentId][i] ) ) //&& (_wm->getEnergyLevel() > 0.0) ) --> always true as status is active
			{
				//BattleAgentObserver* agentObserver = dynamic_cast<BattleAgentObserver*>(gWorld->getAgent(i)->getObserver());
				//if ( ! agentObserver )
				//{
				//	std::cerr << "Error from robot " << _wm->_agentId << " : the observer of robot " << i << " isn't a BattleAgentObserver" << std::endl;
				//	exit(1);
				//}
				//agentObserver->writeGenome(_currentGenome, _wm->_agentId);
			}
		}
	}

	// * handle genome renewal

	//"restart" the robot in case it runs out of energy -- case: no synchronisation
	/*
	if ( ( _wm->getEnergyLevel()  <= 0.0 ) &&  ( MedeaSharedData::gSynchronization == false ) ){
		logStatus();
		//resetActiveGenome();
		_wm->setEnergyLevel( currentBehavior->getInitialEnergy());
		_wm->setDeltaEnergy(0.0);

		gLogFile << "Info(" << gWorld->getIterations() << ") : Human intervention on robot " << _wm->_agentId << " (Energy)" << std::endl;
		_iterationCount = 0;
		_wm->setActiveStatus(false); // !N.20100407 : inactive robot should get a new genome and move until it imports one from neighbors.
	}
	*/
	
	// case: default for Medea, synchronised
	if( _iterationCount >= MedeaSharedData::gEvaluationTime )
	{
		/**/
		if (_wm->_agentId ==  gAgentIndexFocus && gVerbose) // debug
		{
			std::cout << "agent #" << gAgentIndexFocus << " is renewed" << std::endl;
			std::cout << "agent #" << gAgentIndexFocus << " imported " << _genomesList.size() << " genomes. Energy is " << _wm->getEnergyLevel() << ". Status is "  << _wm->getActiveStatus() << "." <<std::endl;
		}
		/**/

	
		logStatus();
		
		//"revive" the robot in case it runs out of energy
		if ( MedeaSharedData::gExperimentNumber == 1 || MedeaSharedData::gExperimentNumber == 2 )
		{
			if  ( _wm->getEnergyLevel()  <= 0.0 )
			{
				gLogFile << "Info(" << gWorld->getIterations() << ") : Human intervention on robot " << _wm->_agentId << " (Energy)" << std::endl;
				// reformulate (check python script compatibility before): gLogFile << "Info(" << gWorld->getIterations() << ") : robot " << _wm->_agentId << " was revived (human intervention)" << std::endl;

				if (_wm->_agentId == gAgentIndexFocus && gVerbose) // debug
				{
					std::cout << "agent #" << gAgentIndexFocus << " is revived (energy was 0)." << std::endl;
				}

				logStatus();
				//resetActiveGenome();
			
				_wm->setEnergyLevel(MedeaSharedData::gEnergyRevive); // !n : too few?

				_wm->setActiveStatus(false); // true: restart, false: no-restart
			
				_genomesList.empty();
			}
		}
		
		//else // uncomment if restart
		// note: at this point, agent got energy, wether because it was revived or because of remaining energy.
		// case: genome(s) imported, random pick.
		if (_genomesList.size() > 0)
		{
			pickRandomGenome();
			_wm->setActiveStatus(true); // !N.20100407 : revive takes imported genome if any
		}
		// case: no imported genome - wait for new genome.
		else
		{
			gLogFile << "Info(" << gWorld->getIterations() << ") : robot nb." << _wm->_agentId
			//					<< " is trying a whole new genome" << std::endl;
								<< " is waiting for a new genome" << std::endl;

			//resetActiveGenome(); // optional -- could be set to zeroes.
			_wm->setActiveStatus(false); // !N.20100407 : inactive robot must import a genome from others.
		}
		
		//log the genome
		gLogFile << "get active status" << std::endl ;
		if ( _wm->getActiveStatus() == true )
		{
			gLogFile << "Info("<< gWorld->getIterations() <<") : robot nb."<< _wm->_agentId << " use genome :";
			for(unsigned int i=0; i<_wm->_genome.size(); i++)
			{
				gLogFile << std::fixed << std::showpoint<< _wm->_genome[i] << " ";
			}
			gLogFile << std::endl;
		}

		//Re-initialize the main parameters

		if ( MedeaSharedData::gExperimentNumber == 1 || MedeaSharedData::gExperimentNumber == 2 )
		{
			_wm->setDeltaEnergy(0.0); // !n : avant: 10.0
		}
		
		_iterationCount = 0;
		_generationCount ++;
		
		
		if ( _wm->_agentId == 0 )
		{
			if ( !gVerbose )
			{
				//std::cout << ".";
				int activeCount = 0;
				for ( int i = 0 ; i != gAgentCounter ; i++ )
				{
					if ( _wm->getActiveStatus() == true )  
						activeCount++;
				}
				std::cout << "[" << activeCount << "]";
			}
				
			
		}
	}


	
}


void BattleAgentObserver::pickRandomGenome()
{
		if(_genomesList.size() != 0)
		{
			int randomIndex = rand()%_genomesList.size();
			std::map<int, std::vector<double> >::iterator it = _genomesList.begin();
			while (randomIndex !=0 )
			{
				it ++;
				randomIndex --;
			}
			_currentGenome = (*it).second;
			_wm->_genome = _currentGenome;
			_wm->setNewGenomeStatus(true);
			gLogFile << "Info(" << gWorld->getIterations() << ") : robot nb." << _wm->_agentId << " take the genome from the robot nb." << (*it).first << std::endl;
			_genomesList.clear();
		}
}

void BattleAgentObserver::writeGenome(std::vector<double> genome, int senderId)
{
	_genomesList[senderId] = genome;
}




void BattleAgentObserver::loadGenome(std::vector<double> inGenome)
{
	std::cout << std::flush ;
	_wm->_genome.clear();
	for ( unsigned int i = 0 ; i != inGenome.size() ; i++ )
	{
		_wm->_genome.push_back(inGenome[i]);
	}
	_currentGenome = _wm->_genome;
	_wm->setNewGenomeStatus(true);
	_genomesList.clear();
//	MedeaPerceptronControlArchitecture* currentBehavior = dynamic_cast<MedeaPerceptronControlArchitecture*>(gWorld->getAgent(_wm->_agentId)->getBehavior());
	_wm->setDeltaEnergy(10.0);
	_wm->setActiveStatus(true); // true: restart, false: no-restart
}

int BattleAgentObserver::getSide()
{
	return _side;
}

void BattleAgentObserver::logStatus()
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
		<< " #DeltaEnergy: " << _wm->getDeltaEnergy()
		<< " #GenomeListSize: " << _genomesList.size() <<std::endl;
}
