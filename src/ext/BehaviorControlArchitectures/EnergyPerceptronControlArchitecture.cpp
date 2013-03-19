/*
 *  EnergyPerceptronControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Jean-Marc on 21/12/2009.
 *
 */

#include "BehaviorControlArchitectures/EnergyPerceptronControlArchitecture.h"

#include "World/World.h"

#include "Utilities/Misc.h"

#include <math.h>

EnergyPerceptronControlArchitecture::EnergyPerceptronControlArchitecture( RobotAgentWorldModel *__wm )
{
	_wm = (EvolvingRobotAgentWorldModel*)__wm;
	//_wm->_genome.resize(18);
	_iteration = 0;
	gProperties.checkAndGetPropertyValue("evaluationTime", &_evaluationTime, true);
	//gProperties.checkAndGetPropertyValue("maxEvaluation", &_maxEvaluation, true);
	//gProperties.checkAndGetPropertyValue("driftEvaluationRate", &_driftEvaluationRate, true);

	_initialEnergy = 2*_evaluationTime*0.1;
	_energy = _initialEnergy;
	_deltaEnergy = 0.0;

	_angleToClosestEnergyPoint = 0;
	_distanceToClosestEnergyPoint = 0;
}

EnergyPerceptronControlArchitecture::~EnergyPerceptronControlArchitecture()
{
	// nothing to do.
}

void EnergyPerceptronControlArchitecture::reset()
{
	_parameters.clear();
	_parameters = _wm->_genome;
}


// perform one controller update
// set motor value using motor format.
void EnergyPerceptronControlArchitecture::step()
{
	_iteration ++;

	if ( _wm->getNewGenomeStatus() ) // check for new NN parameters
	{
		reset();
		_wm->setNewGenomeStatus(false);
	}

	if ( _wm->_age < 0 )
	{
		// ** security control (prior to a new behavior, get out of crash situation) -- random noise to avoid "STALL" status
		_wm->_desiredTranslationalValue = ( ranf()*2.-1. ) * gMaxTranslationalSpeed ;
		_wm->_desiredRotationalVelocity =( ranf()*2.-1. ) * gMaxRotationalSpeed ;
		return;
	}

	_wm->_desiredTranslationalValue = 0;
	_wm->_desiredRotationalVelocity = 0;

	// input to output
	/*
	for ( int i = 0 ; i != _wm->getDefaultSensors()->getSensorCount() ; i++ )
	{
		_wm->_desiredTranslationalValue += _parameters[i] * _wm->getDefaultSensors()->getSensorDistanceValue(i);
		_wm->_desiredRotationalVelocity += _parameters[i+8+1] * _wm->getDefaultSensors()->getSensorDistanceValue(i);
	}

	//if we are in the fist part of the run give a random value
	_angleToClosestEnergyPoint=  rand()% (int)(2.0*M_PI);
	_distanceToClosestEnergyPoint = rand()%1156;
*/

	//use _angleToClosestEnergyPoint and _distanceToClosestEnergyPoint
	_wm->_desiredTranslationalValue += _parameters[8] * _angleToClosestEnergyPoint;
	_wm->_desiredRotationalVelocity +=  _parameters[9] * _distanceToClosestEnergyPoint;
	_wm->_desiredTranslationalValue += _parameters[19] * _angleToClosestEnergyPoint;
	_wm->_desiredRotationalVelocity +=  _parameters[20] * _distanceToClosestEnergyPoint;


	// bias to output
	_wm->_desiredTranslationalValue += _parameters[10] * 1.;
	_wm->_desiredRotationalVelocity +=  _parameters[21] * 1.;

	//_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue)*(1/(1+exp(-0.17*(_deltaEnergy-5))));
	_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue)*(0.1*(_deltaEnergy+20));
	if ( _wm->_desiredTranslationalValue > 1.0 )
	{
		_wm->_desiredTranslationalValue += 1.0;
	}
	if ( _wm->_desiredTranslationalValue < 0.0 )
	{
		_wm->_desiredTranslationalValue += 0.0;
	}
	//_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue);
	_wm->_desiredRotationalVelocity = tanh(_wm->_desiredRotationalVelocity);
	
	// normalize to motor interval values
	_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
	_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;

	//decrease the energyLevel
	_energy -= 0.1;
	_deltaEnergy -= 0.1;
}

float EnergyPerceptronControlArchitecture::getEnergy()
{
	return _energy;
}

void EnergyPerceptronControlArchitecture::setEnergy(float inValue)
{
	_energy = inValue;
	if ( _energy > _initialEnergy)
	{
		_energy = _initialEnergy;
	}
}

float EnergyPerceptronControlArchitecture::getDeltaEnergy()
{
	return _deltaEnergy;
}

void EnergyPerceptronControlArchitecture::setDeltaEnergy(float inValue)
{
	_deltaEnergy = inValue;
}

float EnergyPerceptronControlArchitecture::getInitialEnergy()
{
	return _initialEnergy;
}

void EnergyPerceptronControlArchitecture::searchClosestEnergyPoint(Point2d posRobot)
{
    /*
	std::vector<EnergyPoint>::iterator closestPoint = gEnergyPoints.begin();
	float shortestDistance = getEuclidianDistance (posRobot,closestPoint->getPosition());

	for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin()+1; it < gEnergyPoints.end(); it++)
	{
		float newDistance = getEuclidianDistance (posRobot,it->getPosition());
		if(newDistance < shortestDistance)
		{
			shortestDistance = newDistance;
			closestPoint = it;
		}
	}

	_distanceToClosestEnergyPoint = shortestDistance;

	//compute the angle between the robot and the closest energyPoint
	_angleToClosestEnergyPoint = atan2(closestPoint->getPosition().y,closestPoint->getPosition().x) - atan2(posRobot.y,posRobot.x);

	//place this angle between 0 and 2*M_PI
	if (_angleToClosestEnergyPoint < 0)
	{
		_angleToClosestEnergyPoint += 2.0*M_PI;
	}
    */
}


