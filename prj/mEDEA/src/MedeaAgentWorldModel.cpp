/*
 *  MedeaAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 15/04/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "mEDEA/include/MedeaAgentWorldModel.h"

MedeaAgentWorldModel::MedeaAgentWorldModel()
{
}

MedeaAgentWorldModel::~MedeaAgentWorldModel()
{
}

double MedeaAgentWorldModel::getEnergyPointDirectionAngleValue()
{
	return _energyPointDirectionAngleValue;
}

double MedeaAgentWorldModel::getEnergyPointDistanceValue()
{
	return _energyPointDistanceValue;
}

void MedeaAgentWorldModel::setEnergyPointDirectionAngleValue( double __value )
{
	_energyPointDirectionAngleValue = __value;
}

void MedeaAgentWorldModel::setEnergyPointDistanceValue( double __value )
{
	_energyPointDistanceValue = __value;
}

bool MedeaAgentWorldModel::getActiveStatus()
{
	return _isActive;
}

void MedeaAgentWorldModel::setActiveStatus( bool __isActive )
{
	_isActive = __isActive;
}

double MedeaAgentWorldModel::getEnergyLevel()
{
	return _energyLevel;
}

void MedeaAgentWorldModel::setEnergyLevel(double inValue)
{
	_energyLevel = inValue;
}

double MedeaAgentWorldModel::getDeltaEnergy()
{
	return _deltaEnergy;
}

void MedeaAgentWorldModel::setDeltaEnergy(double inValue)
{
	_deltaEnergy = inValue;
}

void MedeaAgentWorldModel::setAngleToClosestEnergyPoint( double __value )
{
	_angleToClosestEnergyPoint = __value;
}

double MedeaAgentWorldModel::getAngleToClosestEnergyPoint()
{
	return _angleToClosestEnergyPoint;
}
		
void MedeaAgentWorldModel::setDistanceToClosestEnergyPoint( double __value )
{
	_distanceToClosestEnergyPoint = __value;
}

double MedeaAgentWorldModel::getDistanceToClosestEnergyPoint()
{
	return _distanceToClosestEnergyPoint;
}


void MedeaAgentWorldModel::resetActiveGenome()
{
	//int nbInput = _sensorCount + 5 ; //sensors + floorSensor + energyDirection + energyLevel + energyDistance  +bias
//	int nbOutput = 2 ; // rotation + velocity 
	//int nbGene = ( nbInput * _nbHiddenNeurons ) + ( _nbHiddenNeurons * nbOutput) + nbOutput ; // from input to hidden layer + from hiddenLayer to output + from bias to output
	std::cout << std::flush ;
	_genome.clear();
	//for ( int i = 0 ; i != nbGene ; i++ )
	//{
	//	_genome.push_back(((rand()%800)/400.0)-1.0);
	//}
	_currentGenome = _genome;
	setNewGenomeStatus(true);
	_genomesList.clear();
}
