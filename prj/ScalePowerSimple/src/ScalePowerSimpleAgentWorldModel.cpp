
#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"

#include "World/World.h"

ScalePowerSimpleAgentWorldModel::ScalePowerSimpleAgentWorldModel() {
	//_sensors = NULL;
	_energyLevel = 100;
	_active = true;
	_deltaEnergy = 0;
	_energyGained = 0;
}

ScalePowerSimpleAgentWorldModel::~ScalePowerSimpleAgentWorldModel() {
	//if (_sensors != NULL)
	//	delete[] _sensors;
}

void ScalePowerSimpleAgentWorldModel::setActive(bool _active) {
	/*if (this->_active != _active) {
		if (_active) {
			std::cout << "Agent " << this->_agentId << " is now active" << std::endl;
		} else {
			std::cout << "Agent " << this->_agentId << " is now inactive" << std::endl;
		}
	}*/
	this->_active = _active;
}

bool ScalePowerSimpleAgentWorldModel::isActive() const {
	return _active;
}

void ScalePowerSimpleAgentWorldModel::setDeltaEnergy(double _deltaEnergy) {
	this->_deltaEnergy = _deltaEnergy;
}

double ScalePowerSimpleAgentWorldModel::getDeltaEnergy() const {
	return _deltaEnergy;
}

void ScalePowerSimpleAgentWorldModel::setEnergyLevel(double _energyLevel) {
	this->_energyLevel = _energyLevel;
}

double ScalePowerSimpleAgentWorldModel::getEnergyLevel() const {
	return _energyLevel;
}

void ScalePowerSimpleAgentWorldModel::setEnergyGained(double _energygained) {
	this->_energyGained = _energygained;
}

double ScalePowerSimpleAgentWorldModel::getEnergyGained() const {
	return this->_energyGained;
}

void ScalePowerSimpleAgentWorldModel::setAngleToClosestEnergyPoint(double __value) {
	_angleToClosestEnergyPoint = __value;
}

double ScalePowerSimpleAgentWorldModel::getAngleToClosestEnergyPoint() {
	return _angleToClosestEnergyPoint;
}

void ScalePowerSimpleAgentWorldModel::setDistanceToClosestEnergyPoint(double __value) {
	_distanceToClosestEnergyPoint = __value;
}

double ScalePowerSimpleAgentWorldModel::getDistanceToClosestEnergyPoint() {
	return _distanceToClosestEnergyPoint;
}
