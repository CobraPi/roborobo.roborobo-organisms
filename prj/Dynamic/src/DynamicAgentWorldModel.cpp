
#include "Dynamic/include/DynamicAgentWorldModel.h"

#include "World/World.h"


DynamicAgentWorldModel::DynamicAgentWorldModel() {
	_active = true;
	swarmTime = 0;
	organismTime = 0;
	penalty = 0;
    start.x = -1;
    start.y = -1;
    
    typedEnergySensor = new ResourceSensor<TypedEnergyPoint>();
    addSensors(typedEnergySensor);
}

DynamicAgentWorldModel::~DynamicAgentWorldModel() {
    delete typedEnergySensor;
}

void DynamicAgentWorldModel::setActive(bool _active) {
	this->_active = _active;
}

bool DynamicAgentWorldModel::isActive() const {
	return _active;
}

void DynamicAgentWorldModel::setAngleToClosestEnergyPoint(double __value) {
	_angleToClosestEnergyPoint = __value;
}

double DynamicAgentWorldModel::getAngleToClosestEnergyPoint() {
	return _angleToClosestEnergyPoint;
}

void DynamicAgentWorldModel::setDistanceToClosestEnergyPoint(double __value) {
	_distanceToClosestEnergyPoint = __value;
}

double DynamicAgentWorldModel::getDistanceToClosestEnergyPoint() {
	return _distanceToClosestEnergyPoint;
}

ResourceSensor<TypedEnergyPoint> *DynamicAgentWorldModel::getTypedEnergySensor(){
    return typedEnergySensor;
}
