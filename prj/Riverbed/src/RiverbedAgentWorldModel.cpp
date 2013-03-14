
#include "Riverbed/include/RiverbedAgentWorldModel.h"

#include "World/World.h"

RiverbedAgentWorldModel::RiverbedAgentWorldModel() {
	//_sensors = NULL;
	_energyLevel = 100;
	_active = true;
	_deltaEnergy = 0;
	_energyGained = 0;
}

RiverbedAgentWorldModel::~RiverbedAgentWorldModel() {
}

void RiverbedAgentWorldModel::setActive(bool _active) {
	this->_active = _active;
}

bool RiverbedAgentWorldModel::isActive() const {
	return _active;
}

void RiverbedAgentWorldModel::setDeltaEnergy(double __deltaEnergy) {
	this->_deltaEnergy = __deltaEnergy;
}

double RiverbedAgentWorldModel::getDeltaEnergy() const {
	return _deltaEnergy;
}

void RiverbedAgentWorldModel::setEnergyLevel(double __energyLevel) {
	this->_energyLevel = __energyLevel;
}

double RiverbedAgentWorldModel::getEnergyLevel() const {
	return _energyLevel;
}

void RiverbedAgentWorldModel::setEnergyGained(double __energygained) {
	this->_energyGained = __energygained;
}

double RiverbedAgentWorldModel::getEnergyGained() const {
	return this->_energyGained;
}

void RiverbedAgentWorldModel::setAngleToClosestEnergyPoint(double __value) {
	_angleToClosestEnergyPoint = __value;
}

double RiverbedAgentWorldModel::getAngleToClosestEnergyPoint() {
	return _angleToClosestEnergyPoint;
}

void RiverbedAgentWorldModel::setDistanceToClosestEnergyPoint(double __value) {
	_distanceToClosestEnergyPoint = __value;
}

double RiverbedAgentWorldModel::getDistanceToClosestEnergyPoint() {
	return _distanceToClosestEnergyPoint;
}
