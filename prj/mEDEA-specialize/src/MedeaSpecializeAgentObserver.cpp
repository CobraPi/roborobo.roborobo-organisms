/*
 *  MedeaSpecializeAgentObserver.cpp
 *  Roborobo
 *
 *  Created by Nicolas on 15/09/10
 *
 */


#include "mEDEA-specialize/include/MedeaSpecializeAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "mEDEA-specialize/include/MedeaSpecializePerceptronControlArchitecture.h"
#include <cmath>


#include "mEDEA-specialize/include/MedeaSpecializeWorldObserver.h"


// *** *** *** *** ***


MedeaSpecializeAgentObserver::MedeaSpecializeAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (MedeaSpecializeAgentWorldModel*)__wm;

	_wm->_minValue = -1.0;
	_wm->_maxValue = 1.0;

	_wm->_currentSigma = MedeaSpecializeSharedData::gSigmaRef;

	gProperties.checkAndGetPropertyValue("nbHiddenNeurons",&_wm->_nbHiddenNeurons,true);
	
	_wm->resetActiveGenome();
	
	if ( gVerbose )
	{
		std::cout << "robot #" << _wm->_agentId << "\n" ;	
	}
}

MedeaSpecializeAgentObserver::~MedeaSpecializeAgentObserver()
{
	// nothing to do.
}

void MedeaSpecializeAgentObserver::reset()
{
	// nothing to do 
}

void MedeaSpecializeAgentObserver::step()
{
	// debug verbose
	if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
	{
		std::cout << "target: " << _wm->getEnergyPointDirectionAngleValue() << std::endl;
	}

	// * genome spreading (current agent broadcasts its genome to all neighbors - ie. write its own genome in genomeList on neighboring agents)
	// note: could be move in WorldObserver to go from N^2 to Summation(N) complexity
	
	if ( _wm->getActiveStatus() == true )  	// broadcast only if agent is active (ie. not just revived) and deltaE>0.
	{
		for (int i = 0 ; i < gAgentCounter ; i++)
		{
			if ( ( i != _wm->_agentId ) && ( gRadioNetworkArray[_wm->_agentId][i] ) ) //&& (_wm->getEnergyLevel() > 0.0) ) --> always true as status is active
			{/*
				MedeaSpecializeAgentObserver* targetAgentObserver = dynamic_cast<MedeaSpecializeAgentObserver*>(gWorld->getAgent(i)->getObserver());
				if ( ! targetAgentObserver )
				{
					std::cerr << "Error from robot " << _wm->_agentId << " : the observer of robot " << i << " isn't a SwarmOnlineObserver" << std::endl;
					exit(1);
				}
				if ( MedeaSpecializeSharedData::gDynamicSigma == true )
				{
					float dice = float(rand() %100) / 100.0;
					float sigmaSendValue = 0.0;
					if ( ( dice >= 0.0 ) && ( dice < MedeaSpecializeSharedData::gProbAdd) )
					{
						sigmaSendValue = _wm->_currentSigma * ( 1 + MedeaSpecializeSharedData::gDynaStep );
						if (sigmaSendValue > MedeaSpecializeSharedData::gSigmaMax)
						{
							sigmaSendValue = MedeaSpecializeSharedData::gSigmaMax;
						}
					}
					else if ( ( dice >= MedeaSpecializeSharedData::gProbAdd ) && ( dice < MedeaSpecializeSharedData::gProbAdd+MedeaSpecializeSharedData::gProbSub) )
					{
						sigmaSendValue = _wm->_currentSigma * ( 1 - MedeaSpecializeSharedData::gDynaStep );
						if (sigmaSendValue < MedeaSpecializeSharedData::gSigmaMin)
						{
							sigmaSendValue = MedeaSpecializeSharedData::gSigmaMin;
						}
					}
					else
					{
						std::cerr << "Error : In SwarmOnlineObserver, the rand value is out of bound. The sum of MedeaSpecializeSharedData::gProbRef and MedeaSpecializeSharedData::gProbMax is probably not equal to one" << std::endl;
						exit(1);
					}
					targetAgentObserver->writeGenome(_wm->_currentGenome, _wm->_agentId, sigmaSendValue);
				}
				else
				{
					targetAgentObserver->writeGenome(_wm->_currentGenome, _wm->_agentId, _wm->_currentSigma);
				}
				*/
			}
		}
	}

	// * Genome selection/replacement

	if( dynamic_cast<MedeaSpecializeWorldObserver*>(gWorld->getWorldObserver())->getLifeIterationCount() >= MedeaSpecializeSharedData::gEvaluationTime-1 ) // at the end of a generation
	{
		if (_wm->_agentId == gAgentIndexFocus && gVerbose) // debug
		{
			std::cout << "agent #" << gAgentIndexFocus << " is renewed" << std::endl;
			std::cout << "agent #" << gAgentIndexFocus << " imported " << _wm->_genomesList.size() << " genomes. Energy is " << _wm->getEnergyLevel() << ". Status is "  << _wm->getActiveStatus() << "." <<std::endl;
		}

		// Display the current state of the controller
		gLogFile << "#GIteration: " << gWorld->getIterations() << " #Robot: " << _wm->_agentId
				 << " #Energy: " <<  _wm->getEnergyLevel()
				 << " #DeltaEnergy: " << _wm->getDeltaEnergy()
				 << " #GenomeListSize: " << _wm->_genomesList.size() <<std::endl;
			
		// note: at this point, agent got energy, wether because it was revived or because of remaining energy.
		if (_wm->_genomesList.size() > 0)
		{
			// case: 1+ genome(s) imported, random pick.
			
			pickRandomGenome();
			_wm->setActiveStatus(true); // !N.20100407 : revive takes imported genome if any
		}
		else
		{
			// case: no imported genome - wait for new genome.
			
			gLogFile << "Info(" << gWorld->getIterations() << ") : robot nb." << _wm->_agentId
			//					<< " is trying a whole new genome" << std::endl;
								<< " is waiting for a new genome" << std::endl;

			_wm->resetActiveGenome(); // optional -- could be set to zeroes.
			_wm->setActiveStatus(false); // inactive robot *must* import a genome from others (ie. no restart).
		}
		
		//log the genome
		
		if ( _wm->getActiveStatus() == true )
		{
			gLogFile << "Info("<< gWorld->getIterations() <<") : robot nb."<< _wm->_agentId << " use genome :";
			for(unsigned int i=0; i<_wm->_genome.size(); i++)
			{
				gLogFile << std::fixed << std::showpoint << _wm->_genome[i] << " ";
			}
			gLogFile << std::endl;
		}

		//Re-initialize the main parameters

		if ( MedeaSpecializeSharedData::gExperimentNumber == 1 || MedeaSpecializeSharedData::gExperimentNumber == 2 )
		{
			_wm->setDeltaEnergy(0.0); // !n : used to be 10.0
		}
	}	
}



void MedeaSpecializeAgentObserver::pickRandomGenome()
{
		if(_wm->_genomesList.size() != 0)
		{
			int randomIndex = rand()%_wm->_genomesList.size();
			std::map<int, std::vector<double> >::iterator it = _wm->_genomesList.begin();
			while (randomIndex !=0 )
			{
				it ++;
				randomIndex --;
			}

			_wm->_currentGenome = (*it).second;
			if ( MedeaSpecializeSharedData::gDynamicSigma == true )
			{
				mutateWithBouncingBounds(_wm->_sigmaList[(*it).first]);
			}
			else
			{
				mutateWithBouncingBounds(-1.00);
			}
			
			_wm->setNewGenomeStatus(true); 
			gLogFile << "Info(" << gWorld->getIterations() << ") : robot nb." << _wm->_agentId << " take the genome from the robot nb." << (*it).first << std::endl;
			
			if (_wm->_agentId == 1 && gVerbose) // debug
				std::cout << "  Sigma is " << _wm->_sigmaList[(*it).first] << "." << std::endl;
			
			
			_wm->_genomesList.clear();
		}
}



void MedeaSpecializeAgentObserver::writeGenome(std::vector<double> genome, int senderId, float sigma)
{
	_wm->_genomesList[senderId] = genome;
	if ( MedeaSpecializeSharedData::gDynamicSigma == true)
	{
		_wm->_sigmaList[senderId] = sigma;
	}
	/*
	if (_wm->_agentId ==  gAgentIndexFocus ) // debug
	{
		std::cout << "agent #"  << gAgentIndexFocus << " list size is " << _wm->_genomesList.size() << std::endl;
	}
    */
}


void MedeaSpecializeAgentObserver::mutateWithBouncingBounds( float sigma)
{
	_wm->_genome.clear();
	
	if ( MedeaSpecializeSharedData::gDynamicSigma == true)
	{
		_wm->_currentSigma = sigma;
	}
	else
	{
		float randValue = float(rand() %100) / 100.0;
		if ( ( randValue >= 0.0 ) && ( randValue < MedeaSpecializeSharedData::gProbRef) )
		{
			_wm->_currentSigma = MedeaSpecializeSharedData::gSigmaRef;
		}
		else if ( ( randValue >= MedeaSpecializeSharedData::gProbRef ) && ( randValue < MedeaSpecializeSharedData::gProbMax+MedeaSpecializeSharedData::gProbRef) )
		{
			_wm->_currentSigma = MedeaSpecializeSharedData::gSigmaMax;
		}
		else
		{
			std::cerr << "Error : In SwarmOnlineObserver, the rand value is out of bound. The sum of MedeaSpecializeSharedData::gProbRef and MedeaSpecializeSharedData::gProbMax is probably not equal to one" << std::endl;
			exit(1);
		}
	}
	
	for (unsigned int i = 0 ; i != _wm->_currentGenome.size() ; i++ )
	{
		double value = _wm->_currentGenome[i] + getGaussianRand(0,_wm->_currentSigma);
		// bouncing upper/lower bounds
		if ( value < _wm->_minValue )
		{
			double range = _wm->_maxValue - _wm->_minValue;
			double overflow = - ( (double)value - _wm->_minValue );
			overflow = overflow - 2*range * (int)( overflow / (2*range) );
			if ( overflow < range )
				value = _wm->_minValue + overflow;
			else // overflow btw range and range*2
				value = _wm->_minValue + range - (overflow-range);
		}
		else
			if ( value > _wm->_maxValue )
			{
				double range = _wm->_maxValue - _wm->_minValue;
				double overflow = (double)value - _wm->_maxValue;
				overflow = overflow - 2*range * (int)( overflow / (2*range) );
				if ( overflow < range )
					value = _wm->_maxValue - overflow;
				else // overflow btw range and range*2
					value = _wm->_maxValue - range + (overflow-range);
			}
		
		_wm->_genome.push_back(value);
	}
	
	_wm->_currentGenome = _wm->_genome;
	gLogFile << std::fixed << std::showpoint << "Info(" << gWorld->getIterations() << ") : robot nb." << _wm->_agentId << " has mutate the genome with sigma : " << _wm->_currentSigma << std::endl;
}
