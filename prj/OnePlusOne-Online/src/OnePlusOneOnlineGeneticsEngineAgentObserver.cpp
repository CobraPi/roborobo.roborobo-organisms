/*
 *  OnePlusOneOnlineGeneticsControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OnePlusOne-Online/include/OnePlusOneOnlineGeneticsEngineAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"

OnePlusOneOnlineGeneticsEngineAgentObserver::OnePlusOneOnlineGeneticsEngineAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (EvolvingRobotAgentWorldModel*)__wm;
	//std::cout << "robot #" << _wm->_agentId << "\n" ;	
}

OnePlusOneOnlineGeneticsEngineAgentObserver::~OnePlusOneOnlineGeneticsEngineAgentObserver()
{
	// nothing to do.
}

void OnePlusOneOnlineGeneticsEngineAgentObserver::resetActiveGenome()
{
	_wm->_genome.clear();
	for ( int i = 0 ; i != 18 ; i++ )
		_wm->_genome.push_back(getGaussianRand(0,_sigma));
}

void OnePlusOneOnlineGeneticsEngineAgentObserver::reset()
{
	_wm->_age = 0;
	_wm->_generation = 0;

	_storedGenomeFitnessValue = 0;
	_storedGenomeCumulatedFitnessValue = 0;

	_bestEverFitnessValue = 0;
	
	_evaluationCount = 0;
	_reevalStoredGenome = true;
	
	_sigma = 1;

	_sigmaMin = 0.0001;
	_sigmaMax = 4; // 4;

	_minValue = -4; // -4
	_maxValue = +4; // +4
	
	_randomMoveIterations = 5;
	_recoveryIterations   = 15;
	_lifetimeIterations   = 150;

	_reevaluationProbability = 0.2;

	resetActiveGenome();
	
	_storedGenome.clear();
	_storedGenome = _wm->_genome;

	resetFitness();
}

void OnePlusOneOnlineGeneticsEngineAgentObserver::step()
{

	if ( _wm->_age != _lifetimeIterations )
	{
		if ( _wm->_age > _recoveryIterations )  // age<50 means "free of charge"
			updateFitness();
		_wm->_age++;
	}
	else {
		if ( _wm->_age == _lifetimeIterations )
		{				
			// monitoring best behavior with robot no.0 (ie. robot no.0 is not evolving, but always set up with best genome found so far in robot no.1)
			if ( _wm->_agentId == 0 ) // Two robots -- Number 0 is only used for demonstration (always get the best genome wrt. cumulated fitness)
			{
				//OnePlusOneOnlineGeneticsEngineAgentObserver* g = ((OnePlusOneOnlineGeneticsEngineAgentObserver*)((_wm->_world)->getAgent(1))->getObserver());
				//if ( g->_storedGenomeCumulatedFitnessValue > _storedGenomeCumulatedFitnessValue )
				//{
				//	_wm->_genome.clear();
				//	_wm->_genome = g->_storedGenome;
				//	_storedGenomeCumulatedFitnessValue = g->_storedGenomeCumulatedFitnessValue;
				//	//if ( gVerbose)
				//	{
				//	  std::cout << "robot #" << _wm->_agentId << " : " << _storedGenomeCumulatedFitnessValue << " - genome: " ;
				//	  std::cout << "{" ;
					  //for ( int i = 0 ; i != _wm->_sensorCount*2+2 ; i++ )
					  //{
						//  std::cout << _wm->_genome[i];
						//  if ( i != ( ( _wm->_sensorCount*2+2 ) - 1 ) )
						//	  std::cout << ",";
					  //}
				//	  std::cout << "}; " << std::endl;
				//	}
				//}
			}
			else	
			{
				
				// * logging data
				
				bool isNewBest = _wm->_fitnessValue > _storedGenomeFitnessValue ? true : false;
				if ( _bestEverFitnessValue < _wm->_fitnessValue ) 
					_bestEverFitnessValue = _wm->_fitnessValue;
				
				
				// #generation and best fitness ever
				gLogFile << _wm->_generation << ",";
				
				
				// current fitness value for parent, eval counter
				if ( isNewBest )
				{					
					gLogFile << _bestEverFitnessValue << ",";							// best fitness ever (unused)
					gLogFile << _wm->_fitnessValue << ",";			// current storedfitness (new best)
					gLogFile << _wm->_fitnessValue << ",";			// current fitness
				}
				else
				{
					gLogFile << _bestEverFitnessValue << ",";							// best fitness ever (unused)
					if ( _reevalStoredGenome )
						gLogFile << _wm->_fitnessValue << ",";	// stored fitness is updated with current fitness
					else
						gLogFile << _storedGenomeFitnessValue << ",";	// current best fitness (new best)
					gLogFile << _wm->_fitnessValue << ",";			// current fitness
				}
				
				// found new best for *new* genome ?
				if ( isNewBest )
					if ( !_reevalStoredGenome )
					{
						gLogFile << "1" << ",";			// #evaluations
						gLogFile << "1";
					}
					else
					{
						gLogFile << _evaluationCount+1 << ",";			// #evaluations
						gLogFile << "0";
					}
				else
				{
					if ( !_reevalStoredGenome )
						gLogFile << _evaluationCount << ",";			// #evaluations
					else
						gLogFile << _evaluationCount+1 << ",";			// #evaluations
					gLogFile << "0";
				}
				
				
				
				gLogFile << std::endl;

			
				// * (1+1)-online evolutionary algorithm 
				
				if ( _reevalStoredGenome )
				{
					// update stored genome (reevaluated)
					_storedGenomeFitnessValue = _wm->_fitnessValue;
					_storedGenomeCumulatedFitnessValue += _wm->_fitnessValue;
					_reevalStoredGenome = false;
					_evaluationCount++;
				}
				else
				{
					// compare active and stored genome
					if ( _wm->_fitnessValue > _storedGenomeFitnessValue )
					{
						// found new best
						_storedGenomeFitnessValue = _wm->_fitnessValue;
						_storedGenomeCumulatedFitnessValue = _wm->_fitnessValue;
						_storedGenome.clear();
						_storedGenome = _wm->_genome;
						_sigma = _sigmaMin;
						_evaluationCount = 1;
					}
					else
					{
						if ( ranf() < _reevaluationProbability )
						{
							// reevaluate.
							_wm->_genome = _storedGenome;
							_reevalStoredGenome = true;
						}
					}
					if ( _reevalStoredGenome == false )
					{
						// generate new active.
						mutateWithBouncingBounds();
						
						if ( _sigma < _sigmaMax ) _sigma *= 2;
					}
				}		
			}

			// prepare for new evaluation
			resetFitness();
			_wm->setNewGenomeStatus(true);
			
			_wm->_age = -_randomMoveIterations; // age<0 means random mouvement until age=0D
			_wm->_generation++;
		}	
	}
}

void OnePlusOneOnlineGeneticsEngineAgentObserver::resetFitness()
{ 
	_wm->_fitnessValue = 0;
	//_wm->_fitnessValue = INFINITY;
}

void OnePlusOneOnlineGeneticsEngineAgentObserver::updateFitness()
{
	// * fitness: nolfi's book classical fitness
	
//	double minimumCurrentSensorValue = INFINITY;
//	double maximumPossibleSensorValue = 0;

	//for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
	//{
	//	if ( _wm->getSensorDistanceValue(i) < minimumCurrentSensorValue )
	//		minimumCurrentSensorValue = _wm->getSensorDistanceValue(i);
	//	if ( _wm->getSensorMaximumDistanceValue(i) > maximumPossibleSensorValue )
	//		maximumPossibleSensorValue = _wm->getSensorMaximumDistanceValue(i);
	//}

	/*
	// compute actual translation and rotation using speed sensor (ie. simulate odometry)
	double trans = _wm->_actualTranslationalValue < 0 ? -_wm->_actualTranslationalValue : _wm->_actualTranslationalValue;
	double rot = _wm->_actualRotationalVelocity < 0 ? -_wm->_actualRotationalVelocity : _wm->_actualRotationalVelocity;
	double instantFitness =
		( trans / gMaxTranslationalSpeed )
		* ( ( _wm->_maxRotationalDeltaValue - rot ) / _wm->_maxRotationalDeltaValue )
		* ( minimumCurrentSensorValue / maximumPossibleSensorValue );
		// trans * ( _rotationalMaxDeltaValue - rot ) * minimumCurrentSensorValue;
	_wm->_fitnessValue += instantFitness;

	if ( isnan((float)_wm->_fitnessValue) )
	//if ( _wm->_agentId == 1 && _wm->_age%500 == 499 )
	{
		std::cout 
			<< "CRITICAL: fitness value is not a number -- dump info:\n"	
			<< "Instant fitness value = " << instantFitness << "\n"
			<< "Total   fitness value = " << _wm->_fitnessValue << "\n"
			<< "trans: \t\t" << trans << "\t(" << ( trans / gMaxTranslationalSpeed ) <<")\n"
			<< "rot: \t\t\t" << _wm->_actualRotationalVelocity << "\t(" <<  ( ( _wm->_maxRotationalDeltaValue - rot ) / _wm->_maxRotationalDeltaValue )  << ")\n"
			<< "minsensor: \t" <<  minimumCurrentSensorValue << "\t(" << ( minimumCurrentSensorValue / maximumPossibleSensorValue )  << ")" << std::endl; // dont forget that translation delta value is given with tanh (-1 and +1 are common value)
		std::cout << "NN weights: ";
		for ( int i = 0 ; i != _wm->_sensorCount*2+2 ; i++ )
			std::cout << _wm->_genome[i] << " , ";
		std::cout << std::endl;
		std::cout << "sigma: " << _sigma;
		std::cout << std::endl;

	//	exit (-1);
	}	

	// * fitness: min-of-distance-over-life
	// 
	// double distanceToTarget = ( pow((int)(_xReal)-1040 ,2) + pow((int)(_yReal)-320,2) ) ;
	// if ( distanceToTarget < _fitnessValue )
	//	_fitnessValue = distanceToTarget;
	*/
}

double OnePlusOneOnlineGeneticsEngineAgentObserver::getFitnessValue()
{ 
	return -_wm->_fitnessValue;
}


void OnePlusOneOnlineGeneticsEngineAgentObserver::mutateWithBouncingBounds()
{
	_wm->_genome.clear();
	for ( int i = 0 ; i != 18 ; i++ )
	{
		double value = _storedGenome[i] + getGaussianRand(0,_sigma);

		// bouncing upper/lower bounds
		if ( value < _minValue )
		{
			double range = _maxValue - _minValue;
			double overflow = - ( (double)value - _minValue );
			overflow = overflow - 2*range * (int)( overflow / (2*range) );
			if ( overflow < range )
				value = _minValue + overflow;
			else // overflow btw range and range*2
				value = _minValue + range - (overflow-range);
		}
		else
			if ( value > _maxValue )
			{
				double range = _maxValue - _minValue;
				double overflow = (double)value - _maxValue;
				overflow = overflow - 2*range * (int)( overflow / (2*range) );
				if ( overflow < range )
					value = _maxValue - overflow;
				else // overflow btw range and range*2
					value = _maxValue - range + (overflow-range);
			}

		_wm->_genome.push_back(value);
	}
}

