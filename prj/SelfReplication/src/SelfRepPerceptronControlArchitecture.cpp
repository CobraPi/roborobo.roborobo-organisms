/*
 *  SelfRepPerceptronControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Jean-Marc on 21/12/2009.
 *
 */

#include "SelfReplication/include/SelfRepPerceptronControlArchitecture.h"

#include "World/World.h"

#include "Utilities/Misc.h"

#include <math.h>

SelfRepPerceptronControlArchitecture::SelfRepPerceptronControlArchitecture( RobotAgentWorldModel *__wm )
{
	_wm = __wm;
	_selfRepWm = dynamic_cast<SelfRepAgentWorldModel*>(_wm);
	if ( !_selfRepWm )
	{
		std::cout << "Error : The World model send to SelfRepPerceptronControlArchitecture isn't of type SelfRepAgentWorldModel" << std::endl;
		exit(1);
	}

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}

	if ( !gEnergyMode )
	{
		std::cout << "Error : gEnergyMode is false. The SelfRepAgentObserver can't work in those conditions. Would you mind to make an effort and define this vairable ?" << std::endl;
		exit(1);
	}

	_selfRepWm->setAge(1);
	gProperties.checkAndGetPropertyValue("iterationMax", &_iterationMax, true);


	gProperties.checkAndGetPropertyValue("maxKeyRange", &_maxKeyRange, true);

	_minValue = -4.0;
	_maxValue = 4.0;
	_nbGene = 29;

	_angleToClosestEnergyPoint = 0;
	_distanceToClosestEnergyPoint = 0;

	_selfRepWm->setKey( ( ranf()*2.-1. ) * _maxKeyRange);

	resetActiveGenome();
}

SelfRepPerceptronControlArchitecture::~SelfRepPerceptronControlArchitecture()
{
	// nothing to do.
}

void SelfRepPerceptronControlArchitecture::reset()
{
	_currentGenome.clear();
	_currentGenome = _selfRepWm->_genome;
}


// perform one controller update
// set motor value using motor format.
void SelfRepPerceptronControlArchitecture::step()
{
	_selfRepWm->setAge(_selfRepWm->getAge() +1);
	_selfRepWm->setDuplicate(false);
	
	Point2d posRobot(_wm->_xReal,_wm->_yReal);
	if ( gEnergyMode )
	{
		for(std::vector<EnergyPoint>::iterator it = gEnergyPoints.begin(); it != gEnergyPoints.end(); it++)
		{
			if( ( getEuclidianDistance(posRobot,it->getPosition()) < gEnergyPointRadius) && ( fabs(_selfRepWm->getKey() - it->getKey() ) < _selfRepWm->getDeltaKey() ) && (it->getActiveStatus()))
			{
				float loadingEnergy = 5*(1.0/(2.0*sqrt(2.0*M_PI)))*exp(-(pow((_selfRepWm->getKey() - it->getKey()),2.0)/(pow(2.0,2.0))))*gEnergyPointValue;
				_selfRepWm->setEnergy (_selfRepWm->getEnergy() + loadingEnergy);
				_selfRepWm->setDeltaEnergy (_selfRepWm->getDeltaEnergy() + loadingEnergy );
				gLogFile << gWorld->getIterations() << " :: Robot nb." << _selfRepWm->getUniqueAgentId()
					<< " has load " << loadingEnergy  << std::endl;
				it->setActiveStatus(false);
			}
		}
	}

	if ( ( _selfRepWm->getAge() > _iterationMax) || (_selfRepWm->getEnergy() <= 0.0) )
	{
		_selfRepWm->setStillAlive(false);
		return ;
	}

	if ( (_selfRepWm->getAge() % (int(((_selfRepWm->_genome[28]+4.0)/8.0)*_iterationMax)+1) ) == 0)
	{
		_selfRepWm->setDuplicate(true);
	}
	stepPerceptron();
}

void SelfRepPerceptronControlArchitecture::searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints)
{
	std::vector<Point2d>::iterator closestPoint = energyPoints.begin();
	float shortestDistance = getEuclidianDistance(posRobot,*closestPoint);

	for(std::vector<Point2d>::iterator it = energyPoints.begin()+1; it < energyPoints.end(); it++)
	{
		float newDistance = getEuclidianDistance(posRobot,*it);
		if(newDistance < shortestDistance)
		{
			shortestDistance = newDistance;
			closestPoint = it;
		}
	}

	_distanceToClosestEnergyPoint = shortestDistance;

	//compute the angle between the robot and the closest energyPoint
	_angleToClosestEnergyPoint = atan2(closestPoint->y,closestPoint->x) - atan2(posRobot.y,posRobot.x);

	//place this angle between 0 and 2*M_PI
	if (_angleToClosestEnergyPoint < 0)
	{
		_angleToClosestEnergyPoint += 2.0*M_PI;
	}
}

void SelfRepPerceptronControlArchitecture::stepPerceptron()
{
	if ( _selfRepWm->getNewGenomeStatus() ) // check for new NN parameters
	{
		reset();
		_selfRepWm->setNewGenomeStatus(false);
	}

	if ( _wm->_age < 0 )
	{
		// ** security control (prior to a new behavior, get out of crash situation) -- random noise to avoid "STALL" status
		_wm->_desiredTranslationalValue = ( ranf()*2.-1. ) * gMaxTranslationalSpeed ;
		_wm->_desiredRotationalVelocity =( ranf()*2.-1. ) * gMaxRotationalSpeed ;
		_selfRepWm->setKey( ( ranf()*2.-1. ) * _maxKeyRange);
		return;
	}

	_wm->_desiredTranslationalValue = 0;
	_wm->_desiredRotationalVelocity = 0;
	float keyToSet = 0.0;
	if (! _selfRepWm->getDuplicate())
	{
		/*
		// input to output
		for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
		{
			_wm->_desiredTranslationalValue += _currentGenome[i] * _wm->getSensorDistanceValue(i);
			_wm->_desiredRotationalVelocity += _currentGenome[i+8+1] * _wm->getSensorDistanceValue(i);
			//keyToSet += _currentGenome[i+16+2] * _wm->getSensorDistanceValue(i); 
		}

		// bias to output
		_wm->_desiredTranslationalValue += _currentGenome[8] * 1.;
		_wm->_desiredRotationalVelocity +=  _currentGenome[17] * 1.;
		keyToSet += _currentGenome[26] * 1.;
		//std::cout << keyToSet << "::" ;

		//_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue)*(1/(1+exp(-0.17*(_selfRepWm->getDeltaEnergy()-5))));
		_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue);
		_wm->_desiredRotationalVelocity = tanh(_wm->_desiredRotationalVelocity);
		keyToSet = (2/(1+exp(-keyToSet )))-1;
		//keyToSet = (2/(1+exp(-0.005*keyToSet)))-1.0;
		
		// normalize to motor interval values
		_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
		_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;
		keyToSet = keyToSet * _maxKeyRange;
		//std::cout << keyToSet << std::endl;
		 * */

	}
	//decrease the energyLevel
	_selfRepWm->setKey(keyToSet);
	_selfRepWm->setEnergy( _selfRepWm->getEnergy() - 0.1);
	_selfRepWm->setDeltaEnergy( _selfRepWm->getDeltaEnergy() - 0.1);
}

std::vector<double> SelfRepPerceptronControlArchitecture::mutateWithBouncingBounds( float sigma)
{
	std::vector<double> tmpGenome;
	for ( int i = 0 ; i != _nbGene ; i++ )
	{
		double value = _currentGenome[i] + getGaussianRand(0,sigma);
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
		
		tmpGenome.push_back(value);
	}
	return tmpGenome ;
}

void SelfRepPerceptronControlArchitecture::resetActiveGenome()
{
	_selfRepWm->_genome.clear();
	for ( int i = 0 ; i != _nbGene ; i++ )
	{
		_selfRepWm->_genome.push_back(((rand()%800)/100.0)-4.0);
	}
	_currentGenome = _selfRepWm->_genome;
	_selfRepWm->setNewGenomeStatus(true);
}

