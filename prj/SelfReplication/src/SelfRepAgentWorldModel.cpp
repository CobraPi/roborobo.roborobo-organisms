/*
 *  SelfRepAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 22/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SelfReplication/include/SelfRepAgentWorldModel.h"

#include "Utilities/LogManager.h"

int SelfRepAgentWorldModel::_nextId = 0;

SelfRepAgentWorldModel::SelfRepAgentWorldModel()
{
	gProperties.checkAndGetPropertyValue("initialEnergy", &_energy, true);
	_deltaEnergy = 10.0;
	gProperties.checkAndGetPropertyValue("deltaKey", &_deltaKey, true);
	gProperties.checkAndGetPropertyValue("sigmaRef", &_sigmaRef, true);
	_stillAlive = true;
	_duplicate = false;
	_uniqueAgentId = SelfRepAgentWorldModel::_nextId++;
	LogManager::set_path_d("./logs/");
}

SelfRepAgentWorldModel::~SelfRepAgentWorldModel()
{
}

double SelfRepAgentWorldModel::getEnergy()
{
	return _energy;
}

void SelfRepAgentWorldModel::setEnergy(double energy)
{
	_energy = energy;
}

double SelfRepAgentWorldModel::getDeltaEnergy()
{
	return _deltaEnergy;
}

void SelfRepAgentWorldModel::setDeltaEnergy(double deltaEnergy)
{
	_deltaEnergy = deltaEnergy;
}

double SelfRepAgentWorldModel::getDeltaKey()
{
	return _deltaKey;
}

void SelfRepAgentWorldModel::setDeltaKey(double deltaKey)
{
	_deltaKey = deltaKey;
}

double SelfRepAgentWorldModel::getKey()
{
	return _key;
}

void SelfRepAgentWorldModel::setKey(double key)
{
	_key = key;
}

bool SelfRepAgentWorldModel::getStillAlive()
{
	return _stillAlive;
}

void SelfRepAgentWorldModel::setStillAlive(bool stillAlive)
{
	_stillAlive = stillAlive;
}

bool SelfRepAgentWorldModel::getDuplicate()
{
	return _duplicate;
}

void SelfRepAgentWorldModel::setDuplicate(bool duplicate)
{
	_duplicate = duplicate;
}

int SelfRepAgentWorldModel::getUniqueAgentId()
{
	return _uniqueAgentId;
}

int SelfRepAgentWorldModel::getAge()
{
	return _age;
}

void SelfRepAgentWorldModel::setAge(int age)
{
	_age = age;
}
