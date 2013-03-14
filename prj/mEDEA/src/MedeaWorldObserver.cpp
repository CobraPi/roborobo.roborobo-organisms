/*
 *  MedeaWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 24/05/10.
 *  Copyright 2010.
 *
 */

#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"

#include "mEDEA/include/MedeaWorldObserver.h"

#include "World/World.h"


MedeaWorldObserver::MedeaWorldObserver( World* __world ) : WorldObserver( __world )
{
	_world = __world;

	// ==== loading project-specific properties 

	gProperties.checkAndGetPropertyValue("gSwarmOnlineObsUsed",&MedeaSharedData::gSwarmOnlineObsUsed,true);
	gProperties.checkAndGetPropertyValue("gDynamicSigma",&MedeaSharedData::gDynamicSigma,true);
	gProperties.checkAndGetPropertyValue("gSigmaMin",&MedeaSharedData::gSigmaMin,true);
	gProperties.checkAndGetPropertyValue("gProbAdd",&MedeaSharedData::gProbAdd,true);
	gProperties.checkAndGetPropertyValue("gProbSub",&MedeaSharedData::gProbSub,true);
	gProperties.checkAndGetPropertyValue("gDynaStep",&MedeaSharedData::gDynaStep,true);
	gProperties.checkAndGetPropertyValue("gSigmaRef",&MedeaSharedData::gSigmaRef,true);
	gProperties.checkAndGetPropertyValue("gSigmaMax",&MedeaSharedData::gSigmaMax,true);
	gProperties.checkAndGetPropertyValue("gProbRef",&MedeaSharedData::gProbRef,true);
	gProperties.checkAndGetPropertyValue("gProbMax",&MedeaSharedData::gProbMax,true);
	gProperties.checkAndGetPropertyValue("gEvaluationTime",&MedeaSharedData::gEvaluationTime,true);
	gProperties.checkAndGetPropertyValue("gDriftEvaluationRate",&MedeaSharedData::gDriftEvaluationRate,true);
	gProperties.checkAndGetPropertyValue("gInitLock",&MedeaSharedData::gInitLock,true);
	gProperties.checkAndGetPropertyValue("gDriftLock",&MedeaSharedData::gDriftLock,true);
	gProperties.checkAndGetPropertyValue("gMaxKeyRange",&MedeaSharedData::gMaxKeyRange,true);
	gProperties.checkAndGetPropertyValue("gDeltaKey",&MedeaSharedData::gDeltaKey,true);
	gProperties.checkAndGetPropertyValue("gSynchronization",&MedeaSharedData::gSynchronization,true);

	gProperties.checkAndGetPropertyValue("gExperimentNumber",&MedeaSharedData::gExperimentNumber,true);
	gProperties.checkAndGetPropertyValue("gExperiment1_genStart",&MedeaSharedData::gExperiment1_genStart,true);
	gProperties.checkAndGetPropertyValue("gExperiment2_genStart",&MedeaSharedData::gExperiment2_genStart,true);

	gProperties.checkAndGetPropertyValue("g_xStart_EnergyZone",&MedeaSharedData::g_xStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yStart_EnergyZone",&MedeaSharedData::g_yStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_xEnd_EnergyZone",&MedeaSharedData::g_xEnd_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yEnd_EnergyZone",&MedeaSharedData::g_yEnd_EnergyZone,true);

	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxHarvestValue",&MedeaSharedData::gZoneEnergy_maxHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_minHarvestValue",&MedeaSharedData::gZoneEnergy_minHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxFullCapacity",&MedeaSharedData::gZoneEnergy_maxFullCapacity,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_saturateCapacityLevel",&MedeaSharedData::gZoneEnergy_saturateCapacityLevel,true);
	
	gProperties.checkAndGetPropertyValue("gEnergyMax",&MedeaSharedData::gEnergyMax,true);
	gProperties.checkAndGetPropertyValue("gEnergyInit",&MedeaSharedData::gEnergyInit,true);
	gProperties.checkAndGetPropertyValue("gEnergyRevive",&MedeaSharedData::gEnergyRevive,true);
	
	gProperties.checkAndGetPropertyValue("gMaxPenalizationRate",&MedeaSharedData::gMaxPenalizationRate,true);
	if ( MedeaSharedData::gMaxPenalizationRate < 0 || MedeaSharedData::gMaxPenalizationRate > 1 )
	{
		std::cerr << "gMaxPenalizationRate should be defined btw O and 1" << std::endl;
		exit(-1);
	}
	
	gProperties.checkAndGetPropertyValue("gDynamicRespawn",&MedeaSharedData::gDynamicRespawn,true); // forced, in this setup.
	

	// ====

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}
	
	if ( !MedeaSharedData::gSwarmOnlineObsUsed)
	{
		std::cout << "Error : gSwarmOnlineObsUsed == false. The swarm online observer need some variables. Define this option to true or use another observer" << std::endl;
		exit(1);
	}

	if ( !gEnergyMode )
	{
		std::cout << "Error : gEnergyMode should be true to use SwarmOnlineObserver" << std::endl;
		exit(1);
	}
	
	// * iteration and generation counters
	
	_lifeIterationCount = -1;
	_generationCount = -1;

	std::cout << std::endl << "#### Experiment no.0 starts now. ####" << std::endl;
}

MedeaWorldObserver::~MedeaWorldObserver()
{
	// nothing to do.
}

void MedeaWorldObserver::reset()
{
	// nothing to do.
}

void MedeaWorldObserver::step()
{

	// ***
	// * update iteration and generation counters + switch btw experimental setups if required
	// ***
	
	_lifeIterationCount++;

	if( _lifeIterationCount >= MedeaSharedData::gEvaluationTime ) // go to next generation.
	{
		// * monitoring: count number of active agents.

//		int activeCount = 0;
		/*
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			if ( (dynamic_cast<MedeaAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )
				activeCount++;
		}

		if ( !gVerbose )
		{
			std::cout << "[" << activeCount << "]";
		}
	
		gLogFile << "Info(" << gWorld->getIterations() << ") : active count is " << activeCount << std::endl;
	
	
		// * update iterations and generations counters

		_lifeIterationCount = 0;
		_generationCount++;
		
	
		// * Switch btw experiment setups, if required	
	
		updateExperimentalSettings();
		*/

	}


	// * Update environment status wrt. nature and availability of energy resources (update at each iteration)

	updateEnvironmentResources();

	// * update energy level for each agents (ONLY active agents)
	
	updateAllAgentsEnergyLevel();
}


// ***
// * Check and update (ONLY IF REQUIRED) experimental setup.
// ***

void MedeaWorldObserver::updateExperimentalSettings()
{
		if ( _generationCount == MedeaSharedData::gExperiment1_genStart ) 
		{
			std::cout << std::endl << "#### Experiment no.1 starts now. ####" << std::endl;
			MedeaSharedData::gExperimentNumber = 1;
		}
		else
		{
			if ( _generationCount == MedeaSharedData::gExperiment2_genStart )
			{
				std::cout << std::endl << "#### Experiment no.2 starts now. ####" << std::endl;
				
				// * setup new experiment number
			
				MedeaSharedData::gExperimentNumber = 2;
			
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
}

// ***
// * Update environment status wrt. nature and availability of energy resources (update at each iteration)
// ***
void MedeaWorldObserver::updateEnvironmentResources()
{
	if ( MedeaSharedData::gExperimentNumber == 1 && MedeaSharedData::gDynamicRespawn == true )
	{	
	
		// * case: Experiment 1 -- update the energy point respawn lag.
		// sets automaticaly the energy point respawn lag if respawn is selected (ie. gDynamicRespawn = true)

//		int activeCount = 0;
		/*
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			if ( (dynamic_cast<MedeaAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )  
				activeCount++;
		}
		
		// TODO: move parameters to properties file.
		// TODO: WARNING, IGNORE / OVERWRITE gEnergyPointRespawnLagMaxValue
		// (!n)-NOTE: as of 20100527, this code works, but is not modular. create dedicated method for computing update?
		if ( activeCount < 50 )
		{
			gEnergyPointRespawnLagMaxValue = 400;  
		}
		else
		{
			gEnergyPointRespawnLagMaxValue = 72.0 * (double)activeCount - 3200.0; // linear increase from 400 to 4000
			//gEnergyPointRespawnLagMaxValue = 8.0 * (double)activeCount; // linear increase from 400 to 800
		}
		*/
	} 
	else
	{	
		if ( MedeaSharedData::gExperimentNumber == 2 )
		{
			/*
			// * case: Experiment 2 -- monitor number of agents in the energy zone and update available energy.
		
			int agentsOnZone = 0;
			for ( int i = 0 ; i != gAgentCounter ; i++ )
			{
				int x = (int)(gWorld->getAgent(i)->getWorldModel()->getXReal());
				int y = (int)(gWorld->getAgent(i)->getWorldModel()->getYReal());
				// std::cout << "x =" << x << " , y = " << y << std::endl;
	
				if ( x >= MedeaSharedData::g_xStart_EnergyZone && y >= MedeaSharedData::g_yStart_EnergyZone && x <= MedeaSharedData::g_xEnd_EnergyZone && y <= MedeaSharedData::g_yEnd_EnergyZone )
					agentsOnZone++;
			}
		
			if ( gVerbose )
				std::cout << "There are " << agentsOnZone << " agents on the energy zone" <<  std::endl;
		
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
			*/
		}
	}
}


// * 
// * update energy level for each agents (ONLY active agents) (only in experimental setups featuring energy items)
// *
void MedeaWorldObserver::updateAllAgentsEnergyLevel()
{	
	/*
	if ( MedeaSharedData::gExperimentNumber == 1 || MedeaSharedData::gExperimentNumber == 2 ) // only for exp. 1 and 2
	{
		for ( int i = 0 ; i != gAgentCounter ; i++ ) // for each agent
		{
			MedeaAgentWorldModel *currentAgentWorldModel = dynamic_cast<MedeaAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel());
			
			// * check energy level. Becomes inactive if zero.
			
			if ( currentAgentWorldModel->getEnergyLevel() <= 0 )
			{
				currentAgentWorldModel->setDeltaEnergy(0);
				currentAgentWorldModel->setActiveStatus(false);
			}
	
			// * if active, check if agent harvests energy. (experimental setup dependant)
			
			if ( currentAgentWorldModel->getActiveStatus() == true )
			{
				if ( MedeaSharedData::gExperimentNumber == 1 ) 
				{
					// * update agent energy (if needed) - agent should be on an active energy point location to get energy
				
					Point2d posRobot(currentAgentWorldModel->_xReal,currentAgentWorldModel->_yReal);
					if ( gEnergyMode && currentAgentWorldModel->getActiveStatus())
					{
						for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
						{
							if( (getEuclidianDistance (posRobot,it->getPosition()) < gEnergyPointRadius) && (it->getActiveStatus()))
							{
								float loadingEnergy = gEnergyPointValue;
								//float loadingEnergy = 5*(1.0/(2.0*sqrt(2.0*M_PI)))*gEnergyPointValue; // test?
								//float loadingEnergy = 5*(1.0/(2.0*sqrt(2.0*M_PI)))*exp(-(pow((_key - it->getKey()),2.0)/(pow(2.0,2.0))))*gEnergyPointValue;
			
								// update energy level
								currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
								currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
			
								//saturate
								if ( currentAgentWorldModel->getEnergyLevel() > MedeaSharedData::gEnergyMax ) // assume: need MedeaSharedData::gEvaluationTime to live full life
									currentAgentWorldModel->setEnergyLevel(MedeaSharedData::gEnergyMax);
			
								gLogFile << "Info(" << gWorld->getIterations() << ") : " << currentAgentWorldModel->_agentId
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
						// * update agent energy, only if it stands in the energy zone.
					
						if ( currentAgentWorldModel->_xReal >= MedeaSharedData::g_xStart_EnergyZone && currentAgentWorldModel->_xReal <= MedeaSharedData::g_xEnd_EnergyZone && currentAgentWorldModel->_yReal >= MedeaSharedData::g_yStart_EnergyZone && currentAgentWorldModel->_yReal <= MedeaSharedData::g_yEnd_EnergyZone )
						{
							float loadingEnergy = MedeaSharedData::gZoneEnergy_harvestValue;
							
							// update energy level
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
			
							// saturate
							if ( currentAgentWorldModel->getEnergyLevel() > MedeaSharedData::gEnergyMax ) // assume: need MedeaSharedData::gEvaluationTime to live full life
								currentAgentWorldModel->setEnergyLevel(MedeaSharedData::gEnergyMax);
			
							Point2d posRobot(currentAgentWorldModel->_xReal,currentAgentWorldModel->_yReal);				
							gLogFile << "Info(" << gWorld->getIterations() << ") : " << currentAgentWorldModel->_agentId
									<< "(" << posRobot.x << "," << posRobot.y << ")" 
									<< " get an energy point at 0,0 :: Value : " << loadingEnergy  << std::endl; // hack to comply with python log analyser
						}
					}
				}
			}
			
			
			// * update agent energy consumption -- if inactive, "revive" the agent (ie. it ran out of energy)
			
			// decrease the energyLevel and deltaEnergyLevel
			
			if ( currentAgentWorldModel->getEnergyLevel() > 0.0 ) 
			{
				currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel()-1); 
			}
			currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy()-1); 
	
	
			// "revive" agent with empty genomes if ran out of energy.
			
			if  ( currentAgentWorldModel->getEnergyLevel()  <= 0.0 )
			{
				gLogFile << "Info(" << gWorld->getIterations() << ") : Human intervention on robot " << currentAgentWorldModel->_agentId << " (Energy)" << std::endl;
				// reformulate (check python script compatibility before): gLogFile << "Info(" << gWorld->getIterations() << ") : robot " << _wm->_agentId << " was revived (human intervention)" << std::endl;
	
				if (currentAgentWorldModel->_agentId == gAgentIndexFocus && gVerbose) // debug
				{
					std::cout << "agent #" << gAgentIndexFocus << " is revived (energy was 0)." << std::endl;
				}
	
				// Display the current state of the controller
				gLogFile << "#GIteration: " << gWorld->getIterations() << " #Robot: " << currentAgentWorldModel->_agentId
					<< " #Energy: " <<  currentAgentWorldModel->getEnergyLevel()
					<< " #DeltaEnergy: " << currentAgentWorldModel->getDeltaEnergy()
					<< " #GenomeListSize: " << currentAgentWorldModel->_genomesList.size() <<std::endl;
				
				currentAgentWorldModel->resetActiveGenome();
			
				currentAgentWorldModel->setEnergyLevel(MedeaSharedData::gEnergyRevive); // !n : too few?
	
				currentAgentWorldModel->setActiveStatus(false); // true: restart, false: no-restart
			
				currentAgentWorldModel->_genomesList.empty();
			}
		}
		*/
	}


int MedeaWorldObserver::getLifeIterationCount()
{
	return _lifeIterationCount;
}
