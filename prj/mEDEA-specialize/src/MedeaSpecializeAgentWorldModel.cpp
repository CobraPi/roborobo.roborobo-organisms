/*
 *  MedeaSpecializeAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 15/09/10
 *
 */

#include "mEDEA-specialize/include/MedeaSpecializeAgentWorldModel.h"

MedeaSpecializeAgentWorldModel::MedeaSpecializeAgentWorldModel()
{
}

MedeaSpecializeAgentWorldModel::~MedeaSpecializeAgentWorldModel()
{
}

double MedeaSpecializeAgentWorldModel::getEnergyPointDirectionAngleValue()
{
	return _energyPointDirectionAngleValue;
}

double MedeaSpecializeAgentWorldModel::getEnergyPointDistanceValue()
{
	return _energyPointDistanceValue;
}

void MedeaSpecializeAgentWorldModel::setEnergyPointDirectionAngleValue( double __value )
{
	_energyPointDirectionAngleValue = __value;
}

void MedeaSpecializeAgentWorldModel::setEnergyPointDistanceValue( double __value )
{
	_energyPointDistanceValue = __value;
}

bool MedeaSpecializeAgentWorldModel::getActiveStatus()
{
	return _isActive;
}

void MedeaSpecializeAgentWorldModel::setActiveStatus( bool __isActive )
{
	_isActive = __isActive;
}

double MedeaSpecializeAgentWorldModel::getEnergyLevel()
{
	return _energyLevel;
}

void MedeaSpecializeAgentWorldModel::setEnergyLevel(double inValue)
{
	_energyLevel = inValue;
}

double MedeaSpecializeAgentWorldModel::getDeltaEnergy()
{
	return _deltaEnergy;
}

void MedeaSpecializeAgentWorldModel::setDeltaEnergy(double inValue)
{
	_deltaEnergy = inValue;
}

void MedeaSpecializeAgentWorldModel::setAngleToClosestEnergyPoint( double __value )
{
	_angleToClosestEnergyPoint = __value;
}

double MedeaSpecializeAgentWorldModel::getAngleToClosestEnergyPoint()
{
	return _angleToClosestEnergyPoint;
}
		
void MedeaSpecializeAgentWorldModel::setDistanceToClosestEnergyPoint( double __value )
{
	_distanceToClosestEnergyPoint = __value;
}

double MedeaSpecializeAgentWorldModel::getDistanceToClosestEnergyPoint()
{
	return _distanceToClosestEnergyPoint;
}


void MedeaSpecializeAgentWorldModel::resetActiveGenome()
{
	/*
	int nbInput = _sensorCount + 5 ; //sensors + floorSensor + energyDirection + energyLevel + energyDistance  +bias
	int nbOutput = 2 ; // rotation + velocity 
	int nbGene = ( nbInput * _nbHiddenNeurons ) + ( _nbHiddenNeurons * nbOutput) + nbOutput ; // from input to hidden layer + from hiddenLayer to output + from bias to output
	std::cout << std::flush ;
	_genome.clear();
	for ( int i = 0 ; i != nbGene ; i++ )
	{
		_genome.push_back(((rand()%800)/400.0)-1.0);
	}
	_currentGenome = _genome;
	setNewGenomeStatus(true);
	_genomesList.clear();
	*/
}
