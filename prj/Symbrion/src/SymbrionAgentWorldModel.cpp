#include "Symbrion/include/SymbrionAgentWorldModel.h"

#include "World/World.h"

SymbrionAgentWorldModel::SymbrionAgentWorldModel() {
	this->_stateLifetime = 0;
}

SymbrionAgentWorldModel::~SymbrionAgentWorldModel() {
}

void SymbrionAgentWorldModel::setDistanceTraveled( double _distance ) {
	this->_distanceTraveled = _distance;
}

double SymbrionAgentWorldModel::getDistanceTraveled() const {
	return this->_distanceTraveled;
}

void SymbrionAgentWorldModel::setStateLifetime( int lifetime ) {
	this->_stateLifetime = lifetime;
}

int SymbrionAgentWorldModel::getStateLifetime() const {
	return this->_stateLifetime;
}