
#include "ChangingWorld/include/ChangingWorldAgentWorldModel.h"

#include "World/World.h"

ChangingWorldAgentWorldModel::ChangingWorldAgentWorldModel() {
//	_sensors = NULL;
//	_ditchSensor = NULL;
    _active = true;
    _xStart = 0;
    _yStart = 0;
    
    _energyLevel = 0;
	_energyGained = 0;
}

ChangingWorldAgentWorldModel::~ChangingWorldAgentWorldModel() {
}

void ChangingWorldAgentWorldModel::setActive(bool _active) {
	this->_active = _active;
}

bool ChangingWorldAgentWorldModel::isActive() const {
	return _active;
}

void ChangingWorldAgentWorldModel::setEnergyLevel(double _energyLevel) {
	this->_energyLevel = _energyLevel;
}

double ChangingWorldAgentWorldModel::getEnergyLevel() const {
	return _energyLevel;
}

void ChangingWorldAgentWorldModel::setEnergyGained(double _energygained) {
	this->_energyGained = _energygained;
}

double ChangingWorldAgentWorldModel::getEnergyGained() const {
	return this->_energyGained;
}

void ChangingWorldAgentWorldModel::setXStart(double x){
    this->_xStart = x;
}

double ChangingWorldAgentWorldModel::getXStart() const {
    return this->_xStart;
}

void ChangingWorldAgentWorldModel::setYStart(double y){
    this->_yStart = y;
}

double ChangingWorldAgentWorldModel::getYStart() const {
    return this->_yStart;
}

