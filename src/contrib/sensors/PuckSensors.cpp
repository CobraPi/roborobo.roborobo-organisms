/**
 * PuckSensors.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: Job Bakker
 */

#include "Sensor/PuckSensors.h"


PuckSensors::PuckSensors(int id, double originDistance, double originAngle, double targetDistance, double targetAngle, double maxSensorDistance) {
	_id = id;
	_originDistance = originDistance;
	_originAngle = originAngle;
	_targetDistance = targetDistance;
	_targetAngle = targetAngle;
	_sensorRay.maxRange = maxSensorDistance;
}


PuckSensors::~PuckSensors(){

}


void PuckSensors::init(Point2d position, double orientation){

}


void PuckSensors::displaySensorInformation(){

}

void PuckSensors::update(Point2d position, double orientation){
	double orientationAngle = orientation * M_PI / 180;

	double sinOrigin, cosOrigin, sinTarget, cosTarget;
    sinOrigin = std::sin(_originAngle + orientationAngle);
    cosOrigin = std::cos(_originAngle + orientationAngle);
    sinTarget = std::sin(_targetAngle + orientationAngle);
    cosTarget = std::cos(_targetAngle + orientationAngle);
    
	_sensorRay.x1 = position.x + _originDistance * cosOrigin;
	_sensorRay.y1 = position.y + _originDistance * sinOrigin;
	_sensorRay.x2 = position.x + _targetDistance * cosTarget;
	_sensorRay.y2 = position.y + _targetDistance * sinTarget;
}


void PuckSensors::reset(){

}


void PuckSensors::displaySensor(SDL_Surface *surface, Point2d position, double orientation, std::deque<bool> &displayed, bool force) {
    int ranges[50]; int pixels[50];    // (R<<16)+(G<<8)+B; R=objects, B=agents, G=pucks (if R==0xFF)

    int count = castSensorRay(surface, _sensorRay, ranges, pixels);

    // Clear puck ranges
    for (int i = 0; i < MAX_PUCK_COLORS; i++) _puckRanges[i] = _sensorRay.maxRange;
    // Last entry will always be about obstacle collision (or the lack thereof).
    for (int i = 0; i < count - 1; i++) {
        int g = (pixels[i] & 0x00FF00) >> 8;
        _puckRanges[g] = ranges[i];
    }
    _obstacleRange = ranges[count-1];
    _obstacleId = pixels[count-1];
    
    // TODO: Puck sensors are never shown?
}


double PuckSensors::getNormObstacleRange() {
    if (_obstacleRange == _sensorRay.maxRange) return 5.0;
    return 1.0 * _obstacleRange / _sensorRay.maxRange;
}


int PuckSensors::getSensorCount(){
	return MAX_PUCK_COLORS;
}


double PuckSensors::getNormPuckRange(int i) {
    if (_puckRanges[i] == _sensorRay.maxRange) return 5.0;
    return 1.0 * _puckRanges[i] / _sensorRay.maxRange;
}
