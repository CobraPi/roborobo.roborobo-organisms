/*
 *  MedeaSpecializeWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 15/09/10.
 *
 */

#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"

#include "mEDEA-specialize/include/MedeaSpecializeWorldObserver.h"

#include "World/World.h"


MedeaSpecializeWorldObserver::MedeaSpecializeWorldObserver( World* __world ) : WorldObserver( __world )
{

	_world = __world;


	// ==== two-suns specific
	
	gProperties.checkAndGetPropertyValue("gSunLifetime",&MedeaSpecializeSharedData::gSunLifetime,true);


	// ==== loading project-specific properties 

	gProperties.checkAndGetPropertyValue("gSwarmOnlineObsUsed",&MedeaSpecializeSharedData::gSwarmOnlineObsUsed,true);
	gProperties.checkAndGetPropertyValue("gDynamicSigma",&MedeaSpecializeSharedData::gDynamicSigma,true);
	gProperties.checkAndGetPropertyValue("gSigmaMin",&MedeaSpecializeSharedData::gSigmaMin,true);
	gProperties.checkAndGetPropertyValue("gProbAdd",&MedeaSpecializeSharedData::gProbAdd,true);
	gProperties.checkAndGetPropertyValue("gProbSub",&MedeaSpecializeSharedData::gProbSub,true);
	gProperties.checkAndGetPropertyValue("gDynaStep",&MedeaSpecializeSharedData::gDynaStep,true);
	gProperties.checkAndGetPropertyValue("gSigmaRef",&MedeaSpecializeSharedData::gSigmaRef,true);
	gProperties.checkAndGetPropertyValue("gSigmaMax",&MedeaSpecializeSharedData::gSigmaMax,true);
	gProperties.checkAndGetPropertyValue("gProbRef",&MedeaSpecializeSharedData::gProbRef,true);
	gProperties.checkAndGetPropertyValue("gProbMax",&MedeaSpecializeSharedData::gProbMax,true);
	gProperties.checkAndGetPropertyValue("gEvaluationTime",&MedeaSpecializeSharedData::gEvaluationTime,true);
	gProperties.checkAndGetPropertyValue("gDriftEvaluationRate",&MedeaSpecializeSharedData::gDriftEvaluationRate,true);
	gProperties.checkAndGetPropertyValue("gInitLock",&MedeaSpecializeSharedData::gInitLock,true);
	gProperties.checkAndGetPropertyValue("gDriftLock",&MedeaSpecializeSharedData::gDriftLock,true);
	gProperties.checkAndGetPropertyValue("gMaxKeyRange",&MedeaSpecializeSharedData::gMaxKeyRange,true);
	gProperties.checkAndGetPropertyValue("gDeltaKey",&MedeaSpecializeSharedData::gDeltaKey,true);
	gProperties.checkAndGetPropertyValue("gSynchronization",&MedeaSpecializeSharedData::gSynchronization,true);

	gProperties.checkAndGetPropertyValue("gExperimentNumber",&MedeaSpecializeSharedData::gExperimentNumber,true);
	gProperties.checkAndGetPropertyValue("gExperiment1_genStart",&MedeaSpecializeSharedData::gExperiment1_genStart,true);
	gProperties.checkAndGetPropertyValue("gExperiment2_genStart",&MedeaSpecializeSharedData::gExperiment2_genStart,true);

	gProperties.checkAndGetPropertyValue("g_xStart_EnergyZone",&MedeaSpecializeSharedData::g_xStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yStart_EnergyZone",&MedeaSpecializeSharedData::g_yStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_xEnd_EnergyZone",&MedeaSpecializeSharedData::g_xEnd_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yEnd_EnergyZone",&MedeaSpecializeSharedData::g_yEnd_EnergyZone,true);

	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxHarvestValue",&MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_minHarvestValue",&MedeaSpecializeSharedData::gZoneEnergy_minHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxFullCapacity",&MedeaSpecializeSharedData::gZoneEnergy_maxFullCapacity,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_saturateCapacityLevel",&MedeaSpecializeSharedData::gZoneEnergy_saturateCapacityLevel,true);
	
	gProperties.checkAndGetPropertyValue("gEnergyMax",&MedeaSpecializeSharedData::gEnergyMax,true);
	gProperties.checkAndGetPropertyValue("gEnergyInit",&MedeaSpecializeSharedData::gEnergyInit,true);
	gProperties.checkAndGetPropertyValue("gEnergyRevive",&MedeaSpecializeSharedData::gEnergyRevive,true);
	
	gProperties.checkAndGetPropertyValue("gMaxPenalizationRate",&MedeaSpecializeSharedData::gMaxPenalizationRate,true);
	if ( MedeaSpecializeSharedData::gMaxPenalizationRate < 0 || MedeaSpecializeSharedData::gMaxPenalizationRate > 1 )
	{
		std::cerr << "gMaxPenalizationRate should be defined btw O and 1" << std::endl;
		exit(-1);
	}
	
	gProperties.checkAndGetPropertyValue("gDynamicRespawn",&MedeaSpecializeSharedData::gDynamicRespawn,true); // forced, in this setup.
	

	// ====

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}
	
	if ( !MedeaSpecializeSharedData::gSwarmOnlineObsUsed)
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

MedeaSpecializeWorldObserver::~MedeaSpecializeWorldObserver()
{
	// nothing to do.
}

void MedeaSpecializeWorldObserver::reset()
{
	// setting up SUN locations
	
	Point2d positionSun0(24,250);
	Point2d positionSun1(1000,286);

	gEnergyPoints[0].hide();
	gEnergyPoints[0].setFixedLocationStatus(true);
	gEnergyPoints[0].setPosition(positionSun0);
//	gEnergyPoints[0].display();

	gEnergyPoints[1].hide();
	gEnergyPoints[1].setFixedLocationStatus(true);
	gEnergyPoints[1].setPosition(positionSun1);
//	gEnergyPoints[1].display();
	
}

void MedeaSpecializeWorldObserver::step()
{

	if ( gWorld->getIterations() % ( int(MedeaSpecializeSharedData::gEvaluationTime * MedeaSpecializeSharedData::gSunLifetime) ) == 0 )
	{
		gEnergyPoints[MedeaSpecializeSharedData::gActiveSun].hide();
		MedeaSpecializeSharedData::gActiveSun = ( MedeaSpecializeSharedData::gActiveSun + 1 ) % 2;
		std::cout << "\nActive Sun #" << MedeaSpecializeSharedData::gActiveSun << "\n";
		gEnergyPoints[MedeaSpecializeSharedData::gActiveSun].display();
	}



	// ***
	// * update iteration and generation counters + switch btw experimental setups if required
	// ***
	
	_lifeIterationCount++;

	if( _lifeIterationCount >= MedeaSpecializeSharedData::gEvaluationTime ) // go to next generation.
	{
		// * monitoring: count number of active agents.

		int activeCount = 0;
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
		//	if ( (dynamic_cast<MedeaSpecializeAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )
		//		activeCount++;
		}

		if ( !gVerbose )
		{
			std::cout << "[" << activeCount << "]";
		}
	
		gLogFile << "Info(" << gWorld->getIterations() << ") : active count is " << activeCount << std::endl;
	
		
		// * monitor and log orientation and distance to center for all agents
		
		// build heatmap
		
		int heatmapSize = 100; // arbitrary, but should be enough to get precise view
//		int maxDistance = sqrt ( gAreaWidth*gAreaWidth + gAreaHeight*gAreaHeight ) / 2.0 ; // distance max to center.
		
		int *distanceHeatmap;
		distanceHeatmap = new int[heatmapSize];
		int *orientationHeatmap;
		orientationHeatmap = new int[heatmapSize];

		for (int i = 0 ; i != heatmapSize ; i++ )
		{
			distanceHeatmap[i] = 0;
			orientationHeatmap[i] = 0;
		}
			
//		double xRef = gAreaWidth / 2.0 ;
//		double yRef = gAreaHeight / 2.0 ;

		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			//MedeaSpecializeAgentWorldModel *wm = (dynamic_cast<MedeaSpecializeAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()));
			/*if ( wm->getActiveStatus() == true ) // only active agents
			{
				// monitor distance
				
				double dist = getEuclidianDistance( xRef, yRef, wm->_xReal, wm->_yReal );
				int indexDist = (int)dist * heatmapSize / maxDistance; // normalize within heatmap bounds
				distanceHeatmap[indexDist]++;
				
				// monitor orientationHeatmap

				double orient = acos ( ( wm->_xReal - xRef ) / (double)dist );
				if ( wm->_yReal - yRef > 0 ) // [trick] why ">0" ?  : it should be <0, but as the 2D-display norm for the coordinate system origin is upper-left (and not bottom-left), the sign is inversed.
				{
					orient = -orient;
				}
				
				int indexOrient = ( orient + M_PI ) * heatmapSize / (2.0 * M_PI);
				orientationHeatmap[heatmapSize-1-((indexOrient+((heatmapSize*3)/4))%heatmapSize)]++; // index is such that list ordering is as follow: [South-West-North-East-South]
				
				//std::cout << "\n agent[" << wm->_xReal << "," << wm->_yReal << "](" << dist << "," << orient << ") -- " << wm->_xReal << ";" << xRef << ";" << ( ( wm->_xReal - xRef ) / (double)dist ) << ";" << acos ( ( wm->_xReal - xRef ) / (double)dist ); //debug
			}
			*/
		}		
		//std::cout << "\n"; //debug
		
		
		// update log file
		
		std::string str_agentDistancesToRef ="";
		std::string str_agentOrientationsToRef = "";
		
		for (int i = 0 ; i != heatmapSize ; i++ )
		{
			str_agentDistancesToRef += convertToString(distanceHeatmap[i]);
			str_agentDistancesToRef += ",";
			str_agentOrientationsToRef += convertToString(orientationHeatmap[i]);
			str_agentOrientationsToRef += ",";
		}
		
		//gLogFile
		//std::cout << std::endl << std::endl;//debug
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor distance to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentDistancesToRef << std::endl;
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor orientation to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentOrientationsToRef << std::endl;

		for ( int i = 0 ; i != heatmapSize ; i++ )
		{
			gLogFile << "monitorDistance: " << gAgentCounter << "," << (int)sqrt(gMaxRadioDistanceToSquare) << "," << (_generationCount+1) << "," << i << "," << distanceHeatmap[i] << std::endl;
			gLogFile << "monitorOrientation: " << gAgentCounter << "," << (int)sqrt(gMaxRadioDistanceToSquare) << "," << (_generationCount+1) << "," << i << "," << orientationHeatmap[i] << std::endl;
		}
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor distance to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentDistancesToRef << std::endl;
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor orientation to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentOrientationsToRef << std::endl;
				
		delete [] distanceHeatmap;
		delete [] orientationHeatmap;

		// * update iterations and generations counters

		_lifeIterationCount = 0;
		_generationCount++;
		
		// * Switch btw experiment setups, if required	
	
		updateExperimentalSettings();

	}

	// * Update environment status wrt. nature and availability of energy resources (update at each iteration)

	updateEnvironmentResources();

	// * update energy level for each agents (ONLY active agents)
	
	updateAllAgentsEnergyLevel();
	
	// prepape and take screenshot of ultimate iteration
	
	if ( gWorld->getIterations() == gMaxIt-2 )
	{
		gDisplayMode = 0;
		gDisplaySensors = true; // prepape for next it.
	}
	else
	{
		if ( gWorld->getIterations() == gMaxIt-1 )
			saveScreenshot("lastIteration");
	}
	
}



// ***
// * Check and update (ONLY IF REQUIRED) experimental setup.
// ***

void MedeaSpecializeWorldObserver::updateExperimentalSettings()
{
  /*
  ==> NOT USED IN TWO-SUNS -- to be removed
  if ( _generationCount == MedeaSpecializeSharedData::gExperiment1_genStart ) 
		{
			std::cout << std::endl << "#### Experiment no.1 starts now. ####" << std::endl;
			MedeaSpecializeSharedData::gExperimentNumber = 1;
		}
		else
		{
			if ( _generationCount == MedeaSpecializeSharedData::gExperiment2_genStart )
			{
				std::cout << std::endl << "#### Experiment no.2 starts now. ####" << std::endl;
				
				// * setup new experiment number
			
				MedeaSpecializeSharedData::gExperimentNumber = 2;
			
				// * remove energy points.
			
				for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
				{
					it->hide();
				}
				gEnergyPoints.clear();
			
				// * setup new energy zone
			
				Uint32 colorShown = 0xeab71fff;
				Uint32 colorZone  = 0xAAAAAAFF; // for floor sensor.
			
				for (Sint16 xColor = MedeaSpecializeSharedData::g_xStart_EnergyZone ; xColor < MedeaSpecializeSharedData::g_xEnd_EnergyZone ; xColor++)
				{
					for (Sint16 yColor = Sint16 (MedeaSpecializeSharedData::g_yStart_EnergyZone) ; yColor < Sint16 (MedeaSpecializeSharedData::g_yEnd_EnergyZone) ; yColor ++)
					{
							pixelColor(gBackgroundImage, xColor, yColor, colorShown);
							pixelColor(gZoneImage, xColor, yColor, colorZone);
					}
				}
			}
		}
		*/
}

// ***
// * Update environment status wrt. nature and availability of energy resources (update at each iteration)
// ***
void MedeaSpecializeWorldObserver::updateEnvironmentResources()
{
/* 
  ==> NOT USED IN TWO-SUNS -- to be removed
  
  if ( MedeaSpecializeSharedData::gExperimentNumber == 1 && MedeaSpecializeSharedData::gDynamicRespawn == true )
	{	
	
		// * case: Experiment 1 -- update the energy point respawn lag.
		// sets automaticaly the energy point respawn lag if respawn is selected (ie. gDynamicRespawn = true)

		int activeCount = 0;
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			if ( (dynamic_cast<MedeaSpecializeAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )  
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
	} 
	else
	{	
		if ( MedeaSpecializeSharedData::gExperimentNumber == 2 )
		{
		
			// * case: Experiment 2 -- monitor number of agents in the energy zone and update available energy.
		
			int agentsOnZone = 0;
			for ( int i = 0 ; i != gAgentCounter ; i++ )
			{
				int x = (int)(gWorld->getAgent(i)->getWorldModel()->getXReal());
				int y = (int)(gWorld->getAgent(i)->getWorldModel()->getYReal());
				// std::cout << "x =" << x << " , y = " << y << std::endl;
	
				if ( x >= MedeaSpecializeSharedData::g_xStart_EnergyZone && y >= MedeaSpecializeSharedData::g_yStart_EnergyZone && x <= MedeaSpecializeSharedData::g_xEnd_EnergyZone && y <= MedeaSpecializeSharedData::g_yEnd_EnergyZone )
					agentsOnZone++;
			}
		
			if ( gVerbose )
				std::cout << "There are " << agentsOnZone << " agents on the energy zone" <<  std::endl;
		
			if ( agentsOnZone <= MedeaSpecializeSharedData::gZoneEnergy_maxFullCapacity )
			{
				// best case
				MedeaSpecializeSharedData::gZoneEnergy_harvestValue = MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue;
			}
			else
			{
				if ( agentsOnZone <= MedeaSpecializeSharedData::gZoneEnergy_saturateCapacityLevel )
				{
					double energyValueSpan = MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue - MedeaSpecializeSharedData::gZoneEnergy_minHarvestValue;
					int agentsOverheadCount = MedeaSpecializeSharedData::gZoneEnergy_saturateCapacityLevel - MedeaSpecializeSharedData::gZoneEnergy_maxFullCapacity;
					double costPerAgents = energyValueSpan / (double)agentsOverheadCount;
					MedeaSpecializeSharedData::gZoneEnergy_harvestValue = MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue - costPerAgents * ( agentsOnZone- MedeaSpecializeSharedData::gZoneEnergy_maxFullCapacity ) ;
				}
				else 
				{
					// worst case
					MedeaSpecializeSharedData::gZoneEnergy_harvestValue = MedeaSpecializeSharedData::gZoneEnergy_minHarvestValue;
				}
			}
		}
	}
	*/
}


// * 
// * update energy level for each agents (ONLY active agents) (only in experimental setups featuring energy items)
// *
void MedeaSpecializeWorldObserver::updateAllAgentsEnergyLevel()
{	
	/* 
	
	  ==> NOT USED IN TWO-SUNS -- to be removed


	if ( MedeaSpecializeSharedData::gExperimentNumber == 1 || MedeaSpecializeSharedData::gExperimentNumber == 2 ) // only for exp. 1 and 2
	{
		for ( int i = 0 ; i != gAgentCounter ; i++ ) // for each agent
		{
			MedeaSpecializeAgentWorldModel *currentAgentWorldModel = dynamic_cast<MedeaSpecializeAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel());
			
			// * check energy level. Becomes inactive if zero.
			
			if ( currentAgentWorldModel->getEnergyLevel() <= 0 )
			{
				currentAgentWorldModel->setDeltaEnergy(0);
				currentAgentWorldModel->setActiveStatus(false);
			}
	
			// * if active, check if agent harvests energy. (experimental setup dependant)
			
			if ( currentAgentWorldModel->getActiveStatus() == true )
			{
				if ( MedeaSpecializeSharedData::gExperimentNumber == 1 ) 
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
								if ( currentAgentWorldModel->getEnergyLevel() > MedeaSpecializeSharedData::gEnergyMax ) // assume: need MedeaSpecializeSharedData::gEvaluationTime to live full life
									currentAgentWorldModel->setEnergyLevel(MedeaSpecializeSharedData::gEnergyMax);
			
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
					if ( MedeaSpecializeSharedData::gExperimentNumber == 2 ) 
					{
						// * update agent energy, only if it stands in the energy zone.
					
						if ( currentAgentWorldModel->_xReal >= MedeaSpecializeSharedData::g_xStart_EnergyZone && currentAgentWorldModel->_xReal <= MedeaSpecializeSharedData::g_xEnd_EnergyZone && currentAgentWorldModel->_yReal >= MedeaSpecializeSharedData::g_yStart_EnergyZone && currentAgentWorldModel->_yReal <= MedeaSpecializeSharedData::g_yEnd_EnergyZone )
						{
							float loadingEnergy = MedeaSpecializeSharedData::gZoneEnergy_harvestValue;
							
							// update energy level
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
			
							// saturate
							if ( currentAgentWorldModel->getEnergyLevel() > MedeaSpecializeSharedData::gEnergyMax ) // assume: need MedeaSpecializeSharedData::gEvaluationTime to live full life
								currentAgentWorldModel->setEnergyLevel(MedeaSpecializeSharedData::gEnergyMax);
			
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
			
				currentAgentWorldModel->setEnergyLevel(MedeaSpecializeSharedData::gEnergyRevive); // !n : too few?
	
				currentAgentWorldModel->setActiveStatus(false); // true: restart, false: no-restart
			
				currentAgentWorldModel->_genomesList.empty();
			}
		}	
	}
	*/
}


int MedeaSpecializeWorldObserver::getLifeIterationCount()
{
	return _lifeIterationCount;
}
