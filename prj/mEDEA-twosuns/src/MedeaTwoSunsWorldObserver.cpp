/*
 *  MedeaTwoSunsWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 24/05/10.
 *  Copyright 2010.
 *
 */

#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"

#include "mEDEA-twosuns/include/MedeaTwoSunsWorldObserver.h"

#include "World/World.h"


MedeaTwoSunsWorldObserver::MedeaTwoSunsWorldObserver( World* __world ) : WorldObserver( __world )
{

	_world = __world;


	// ==== two-suns specific
	
	gProperties.checkAndGetPropertyValue("gSunLifetime",&MedeaTwoSunsSharedData::gSunLifetime,true);


	// ==== loading project-specific properties 

	gProperties.checkAndGetPropertyValue("gSwarmOnlineObsUsed",&MedeaTwoSunsSharedData::gSwarmOnlineObsUsed,true);
	gProperties.checkAndGetPropertyValue("gDynamicSigma",&MedeaTwoSunsSharedData::gDynamicSigma,true);
	gProperties.checkAndGetPropertyValue("gSigmaMin",&MedeaTwoSunsSharedData::gSigmaMin,true);
	gProperties.checkAndGetPropertyValue("gProbAdd",&MedeaTwoSunsSharedData::gProbAdd,true);
	gProperties.checkAndGetPropertyValue("gProbSub",&MedeaTwoSunsSharedData::gProbSub,true);
	gProperties.checkAndGetPropertyValue("gDynaStep",&MedeaTwoSunsSharedData::gDynaStep,true);
	gProperties.checkAndGetPropertyValue("gSigmaRef",&MedeaTwoSunsSharedData::gSigmaRef,true);
	gProperties.checkAndGetPropertyValue("gSigmaMax",&MedeaTwoSunsSharedData::gSigmaMax,true);
	gProperties.checkAndGetPropertyValue("gProbRef",&MedeaTwoSunsSharedData::gProbRef,true);
	gProperties.checkAndGetPropertyValue("gProbMax",&MedeaTwoSunsSharedData::gProbMax,true);
	gProperties.checkAndGetPropertyValue("gEvaluationTime",&MedeaTwoSunsSharedData::gEvaluationTime,true);
	gProperties.checkAndGetPropertyValue("gDriftEvaluationRate",&MedeaTwoSunsSharedData::gDriftEvaluationRate,true);
	gProperties.checkAndGetPropertyValue("gInitLock",&MedeaTwoSunsSharedData::gInitLock,true);
	gProperties.checkAndGetPropertyValue("gDriftLock",&MedeaTwoSunsSharedData::gDriftLock,true);
	gProperties.checkAndGetPropertyValue("gMaxKeyRange",&MedeaTwoSunsSharedData::gMaxKeyRange,true);
	gProperties.checkAndGetPropertyValue("gDeltaKey",&MedeaTwoSunsSharedData::gDeltaKey,true);
	gProperties.checkAndGetPropertyValue("gSynchronization",&MedeaTwoSunsSharedData::gSynchronization,true);

	gProperties.checkAndGetPropertyValue("gExperimentNumber",&MedeaTwoSunsSharedData::gExperimentNumber,true);
	gProperties.checkAndGetPropertyValue("gExperiment1_genStart",&MedeaTwoSunsSharedData::gExperiment1_genStart,true);
	gProperties.checkAndGetPropertyValue("gExperiment2_genStart",&MedeaTwoSunsSharedData::gExperiment2_genStart,true);

	gProperties.checkAndGetPropertyValue("g_xStart_EnergyZone",&MedeaTwoSunsSharedData::g_xStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yStart_EnergyZone",&MedeaTwoSunsSharedData::g_yStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_xEnd_EnergyZone",&MedeaTwoSunsSharedData::g_xEnd_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yEnd_EnergyZone",&MedeaTwoSunsSharedData::g_yEnd_EnergyZone,true);

	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxHarvestValue",&MedeaTwoSunsSharedData::gZoneEnergy_maxHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_minHarvestValue",&MedeaTwoSunsSharedData::gZoneEnergy_minHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxFullCapacity",&MedeaTwoSunsSharedData::gZoneEnergy_maxFullCapacity,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_saturateCapacityLevel",&MedeaTwoSunsSharedData::gZoneEnergy_saturateCapacityLevel,true);
	
	gProperties.checkAndGetPropertyValue("gEnergyMax",&MedeaTwoSunsSharedData::gEnergyMax,true);
	gProperties.checkAndGetPropertyValue("gEnergyInit",&MedeaTwoSunsSharedData::gEnergyInit,true);
	gProperties.checkAndGetPropertyValue("gEnergyRevive",&MedeaTwoSunsSharedData::gEnergyRevive,true);
	
	gProperties.checkAndGetPropertyValue("gMaxPenalizationRate",&MedeaTwoSunsSharedData::gMaxPenalizationRate,true);
	if ( MedeaTwoSunsSharedData::gMaxPenalizationRate < 0 || MedeaTwoSunsSharedData::gMaxPenalizationRate > 1 )
	{
		std::cerr << "gMaxPenalizationRate should be defined btw O and 1" << std::endl;
		exit(-1);
	}
	
	gProperties.checkAndGetPropertyValue("gDynamicRespawn",&MedeaTwoSunsSharedData::gDynamicRespawn,true); // forced, in this setup.
	

	// ====

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}
	
	if ( !MedeaTwoSunsSharedData::gSwarmOnlineObsUsed)
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

MedeaTwoSunsWorldObserver::~MedeaTwoSunsWorldObserver()
{
	// nothing to do.
}

void MedeaTwoSunsWorldObserver::reset()
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

void MedeaTwoSunsWorldObserver::step()
{

	if ( gWorld->getIterations() % ( int(MedeaTwoSunsSharedData::gEvaluationTime * MedeaTwoSunsSharedData::gSunLifetime) ) == 0 )
	{
		gEnergyPoints[MedeaTwoSunsSharedData::gActiveSun].hide();
		MedeaTwoSunsSharedData::gActiveSun = ( MedeaTwoSunsSharedData::gActiveSun + 1 ) % 2;
		std::cout << "\nActive Sun #" << MedeaTwoSunsSharedData::gActiveSun << "\n";
		gEnergyPoints[MedeaTwoSunsSharedData::gActiveSun].display();
	}



	// ***
	// * update iteration and generation counters + switch btw experimental setups if required
	// ***
	
	_lifeIterationCount++;

	if( _lifeIterationCount >= MedeaTwoSunsSharedData::gEvaluationTime ) // go to next generation.
	{
		// * monitoring: count number of active agents.

		int activeCount = 0;
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			//if ( (dynamic_cast<MedeaTwoSunsAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )
			//	activeCount++;
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
			/*MedeaTwoSunsAgentWorldModel *wm = (dynamic_cast<MedeaTwoSunsAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()));
			if ( wm->getActiveStatus() == true ) // only active agents
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
			}*/
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

void MedeaTwoSunsWorldObserver::updateExperimentalSettings()
{
  /*
  ==> NOT USED IN TWO-SUNS -- to be removed
  if ( _generationCount == MedeaTwoSunsSharedData::gExperiment1_genStart ) 
		{
			std::cout << std::endl << "#### Experiment no.1 starts now. ####" << std::endl;
			MedeaTwoSunsSharedData::gExperimentNumber = 1;
		}
		else
		{
			if ( _generationCount == MedeaTwoSunsSharedData::gExperiment2_genStart )
			{
				std::cout << std::endl << "#### Experiment no.2 starts now. ####" << std::endl;
				
				// * setup new experiment number
			
				MedeaTwoSunsSharedData::gExperimentNumber = 2;
			
				// * remove energy points.
			
				for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
				{
					it->hide();
				}
				gEnergyPoints.clear();
			
				// * setup new energy zone
			
				Uint32 colorShown = 0xeab71fff;
				Uint32 colorZone  = 0xAAAAAAFF; // for floor sensor.
			
				for (Sint16 xColor = MedeaTwoSunsSharedData::g_xStart_EnergyZone ; xColor < MedeaTwoSunsSharedData::g_xEnd_EnergyZone ; xColor++)
				{
					for (Sint16 yColor = Sint16 (MedeaTwoSunsSharedData::g_yStart_EnergyZone) ; yColor < Sint16 (MedeaTwoSunsSharedData::g_yEnd_EnergyZone) ; yColor ++)
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
void MedeaTwoSunsWorldObserver::updateEnvironmentResources()
{
/* 
  ==> NOT USED IN TWO-SUNS -- to be removed
  
  if ( MedeaTwoSunsSharedData::gExperimentNumber == 1 && MedeaTwoSunsSharedData::gDynamicRespawn == true )
	{	
	
		// * case: Experiment 1 -- update the energy point respawn lag.
		// sets automaticaly the energy point respawn lag if respawn is selected (ie. gDynamicRespawn = true)

		int activeCount = 0;
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			if ( (dynamic_cast<MedeaTwoSunsAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )  
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
		if ( MedeaTwoSunsSharedData::gExperimentNumber == 2 )
		{
		
			// * case: Experiment 2 -- monitor number of agents in the energy zone and update available energy.
		
			int agentsOnZone = 0;
			for ( int i = 0 ; i != gAgentCounter ; i++ )
			{
				int x = (int)(gWorld->getAgent(i)->getWorldModel()->getXReal());
				int y = (int)(gWorld->getAgent(i)->getWorldModel()->getYReal());
				// std::cout << "x =" << x << " , y = " << y << std::endl;
	
				if ( x >= MedeaTwoSunsSharedData::g_xStart_EnergyZone && y >= MedeaTwoSunsSharedData::g_yStart_EnergyZone && x <= MedeaTwoSunsSharedData::g_xEnd_EnergyZone && y <= MedeaTwoSunsSharedData::g_yEnd_EnergyZone )
					agentsOnZone++;
			}
		
			if ( gVerbose )
				std::cout << "There are " << agentsOnZone << " agents on the energy zone" <<  std::endl;
		
			if ( agentsOnZone <= MedeaTwoSunsSharedData::gZoneEnergy_maxFullCapacity )
			{
				// best case
				MedeaTwoSunsSharedData::gZoneEnergy_harvestValue = MedeaTwoSunsSharedData::gZoneEnergy_maxHarvestValue;
			}
			else
			{
				if ( agentsOnZone <= MedeaTwoSunsSharedData::gZoneEnergy_saturateCapacityLevel )
				{
					double energyValueSpan = MedeaTwoSunsSharedData::gZoneEnergy_maxHarvestValue - MedeaTwoSunsSharedData::gZoneEnergy_minHarvestValue;
					int agentsOverheadCount = MedeaTwoSunsSharedData::gZoneEnergy_saturateCapacityLevel - MedeaTwoSunsSharedData::gZoneEnergy_maxFullCapacity;
					double costPerAgents = energyValueSpan / (double)agentsOverheadCount;
					MedeaTwoSunsSharedData::gZoneEnergy_harvestValue = MedeaTwoSunsSharedData::gZoneEnergy_maxHarvestValue - costPerAgents * ( agentsOnZone- MedeaTwoSunsSharedData::gZoneEnergy_maxFullCapacity ) ;
				}
				else 
				{
					// worst case
					MedeaTwoSunsSharedData::gZoneEnergy_harvestValue = MedeaTwoSunsSharedData::gZoneEnergy_minHarvestValue;
				}
			}
		}
	}
	*/
}


// * 
// * update energy level for each agents (ONLY active agents) (only in experimental setups featuring energy items)
// *
void MedeaTwoSunsWorldObserver::updateAllAgentsEnergyLevel()
{	
	/* 
	
	  ==> NOT USED IN TWO-SUNS -- to be removed


	if ( MedeaTwoSunsSharedData::gExperimentNumber == 1 || MedeaTwoSunsSharedData::gExperimentNumber == 2 ) // only for exp. 1 and 2
	{
		for ( int i = 0 ; i != gAgentCounter ; i++ ) // for each agent
		{
			MedeaTwoSunsAgentWorldModel *currentAgentWorldModel = dynamic_cast<MedeaTwoSunsAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel());
			
			// * check energy level. Becomes inactive if zero.
			
			if ( currentAgentWorldModel->getEnergyLevel() <= 0 )
			{
				currentAgentWorldModel->setDeltaEnergy(0);
				currentAgentWorldModel->setActiveStatus(false);
			}
	
			// * if active, check if agent harvests energy. (experimental setup dependant)
			
			if ( currentAgentWorldModel->getActiveStatus() == true )
			{
				if ( MedeaTwoSunsSharedData::gExperimentNumber == 1 ) 
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
								if ( currentAgentWorldModel->getEnergyLevel() > MedeaTwoSunsSharedData::gEnergyMax ) // assume: need MedeaTwoSunsSharedData::gEvaluationTime to live full life
									currentAgentWorldModel->setEnergyLevel(MedeaTwoSunsSharedData::gEnergyMax);
			
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
					if ( MedeaTwoSunsSharedData::gExperimentNumber == 2 ) 
					{
						// * update agent energy, only if it stands in the energy zone.
					
						if ( currentAgentWorldModel->_xReal >= MedeaTwoSunsSharedData::g_xStart_EnergyZone && currentAgentWorldModel->_xReal <= MedeaTwoSunsSharedData::g_xEnd_EnergyZone && currentAgentWorldModel->_yReal >= MedeaTwoSunsSharedData::g_yStart_EnergyZone && currentAgentWorldModel->_yReal <= MedeaTwoSunsSharedData::g_yEnd_EnergyZone )
						{
							float loadingEnergy = MedeaTwoSunsSharedData::gZoneEnergy_harvestValue;
							
							// update energy level
							currentAgentWorldModel->setEnergyLevel(currentAgentWorldModel->getEnergyLevel() + loadingEnergy);
							currentAgentWorldModel->setDeltaEnergy(currentAgentWorldModel->getDeltaEnergy() + loadingEnergy);
			
							// saturate
							if ( currentAgentWorldModel->getEnergyLevel() > MedeaTwoSunsSharedData::gEnergyMax ) // assume: need MedeaTwoSunsSharedData::gEvaluationTime to live full life
								currentAgentWorldModel->setEnergyLevel(MedeaTwoSunsSharedData::gEnergyMax);
			
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
			
				currentAgentWorldModel->setEnergyLevel(MedeaTwoSunsSharedData::gEnergyRevive); // !n : too few?
	
				currentAgentWorldModel->setActiveStatus(false); // true: restart, false: no-restart
			
				currentAgentWorldModel->_genomesList.empty();
			}
		}	
	}
	*/
}


int MedeaTwoSunsWorldObserver::getLifeIterationCount()
{
	return _lifeIterationCount;
}
