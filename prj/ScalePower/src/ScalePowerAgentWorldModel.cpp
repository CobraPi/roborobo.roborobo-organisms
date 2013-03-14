
#include "ScalePower/include/ScalePowerAgentWorldModel.h"

#include "World/World.h"

ScalePowerAgentWorldModel::ScalePowerAgentWorldModel() {
	//_sensors = NULL;
	_energyLevel = 100;
	_active = true;
	_deltaEnergy = 0;
	_energyGained = 0;
}

ScalePowerAgentWorldModel::~ScalePowerAgentWorldModel() {
}

void ScalePowerAgentWorldModel::setActive(bool _active) {
	/*if (this->_active != _active) {
		if (_active) {
			std::cout << "Agent " << this->_agentId << " is now active" << std::endl;
		} else {
			std::cout << "Agent " << this->_agentId << " is now inactive" << std::endl;
		}
	}*/
	this->_active = _active;
}

bool ScalePowerAgentWorldModel::isActive() const {
	return _active;
}

void ScalePowerAgentWorldModel::setDeltaEnergy(double _deltaEnergy) {
	this->_deltaEnergy = _deltaEnergy;
}

double ScalePowerAgentWorldModel::getDeltaEnergy() const {
	return _deltaEnergy;
}

void ScalePowerAgentWorldModel::setEnergyLevel(double _energyLevel) {
	this->_energyLevel = _energyLevel;
}

double ScalePowerAgentWorldModel::getEnergyLevel() const {
	return _energyLevel;
}

void ScalePowerAgentWorldModel::setEnergyGained(double _energygained) {
	this->_energyGained = _energygained;
}

double ScalePowerAgentWorldModel::getEnergyGained() const {
	return this->_energyGained;
}

void ScalePowerAgentWorldModel::setAngleToClosestEnergyPoint(double __value) {
	_angleToClosestEnergyPoint = __value;
}

double ScalePowerAgentWorldModel::getAngleToClosestEnergyPoint() {
	return _angleToClosestEnergyPoint;
}

void ScalePowerAgentWorldModel::setDistanceToClosestEnergyPoint(double __value) {
	_distanceToClosestEnergyPoint = __value;
}

double ScalePowerAgentWorldModel::getDistanceToClosestEnergyPoint() {
	return _distanceToClosestEnergyPoint;
}
