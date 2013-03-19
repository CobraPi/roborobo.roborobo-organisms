/**
 *  RobotAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldModels/RobotAgentWorldModel.h"

#include "World/World.h"

#include "RoboroboMain/roborobo.h"

#include "Sensor/Sensor.h"
#include "Sensor/DefaultSensors.h"



RobotAgentWorldModel::RobotAgentWorldModel()
{
	_robotLED_status = false; // default: LED is off.
	_robotLED_green = _robotLED_blue = 0; // default value (grey)
	_robotLED_red = 255;
	_agentAbsoluteOrientation = 0;
	_agentAbsoluteLinearSpeed  = 0;
	_actualRotationalVelocity = 0.0;
	_actualTranslationalValue = 0.0;

    _energyLevel = 100;
    _deltaEnergy = 0;
	_energyGained = 0;

    joinedOrganism = false;
    
    position = Point2d(0,0);
    
	/**
	 * Adds the 'default' sensor methods implemented by this class as active
	 * sensor methods.
	 */
	defaultSensors = new DefaultSensors();
	addSensors(defaultSensors);
}



RobotAgentWorldModel::~RobotAgentWorldModel()
{
    sensors.clear();
    delete defaultSensors;
}

void RobotAgentWorldModel::setRobotNeighborhoodCounter( int __value )
{
	_robotNeighborhoodCounter = __value;
}

void RobotAgentWorldModel::incRobotNeighborhoodCounter( ) // increment
{
	_robotNeighborhoodCounter++;
}

int RobotAgentWorldModel::getRobotNeighborhoodCounter()
{
	return _robotNeighborhoodCounter;
}

//double RobotAgentWorldModel::getXReal()
//{
//	return _xReal;
//}
//
//double RobotAgentWorldModel::getYReal()
//{
//	return _yReal;
//}

bool RobotAgentWorldModel::getRobotLED_status()
{
	return _robotLED_status;
}

void RobotAgentWorldModel::setRobotLED_status(bool __status)
{
	_robotLED_status = __status;
}

int RobotAgentWorldModel::getRobotLED_redValue()
{
	return _robotLED_red;
}

int RobotAgentWorldModel::getRobotLED_greenValue()
{
	return _robotLED_green;
}

int RobotAgentWorldModel::getRobotLED_blueValue()
{
	return _robotLED_blue;
}

void RobotAgentWorldModel::setRobotLED_colorValues(int __r, int __g, int __b)
{
	_robotLED_red = __r;
	_robotLED_green = __g;
	_robotLED_blue = __b;
}

void RobotAgentWorldModel::setDeltaEnergy(double __deltaEnergy) {
	this->_deltaEnergy = __deltaEnergy;
}

double RobotAgentWorldModel::getDeltaEnergy() const {
	return _deltaEnergy;
}

void RobotAgentWorldModel::setEnergyLevel(double __energyLevel) {
	this->_energyLevel = __energyLevel;
}

double RobotAgentWorldModel::getEnergyLevel() const {
	return _energyLevel;
}

void RobotAgentWorldModel::setEnergyGained(double __energygained) {
	this->_energyGained = __energygained;
}

double RobotAgentWorldModel::getEnergyGained() const {
	return this->_energyGained;
}

void RobotAgentWorldModel::addSensors(Sensor *sensorType){
	sensors.push_back(sensorType);
}


double RobotAgentWorldModel::getAbsoluteOrientation(){
	return _agentAbsoluteOrientation;
}

/**
 * Initializes the normal obstacle detection sensors
 *
 * All programs likely need obstacle avoidance sensors. However, some
 * programs might need additional sensors, like Puck or EnergyPoint detection
 * sensors. Those sensors should be declared in your own project folder (in /prj).
 * You can implement the abstract sensor class to do this and add your sensor
 * via the addSensor() method in this class.
 *
 * The object avoidance sensors are standard implemented and part of the core
 */
void RobotAgentWorldModel::initSensors(){
	for(unsigned int i = 0; i < sensors.size(); i++){
		std::cout << "passed sensor nr.: " << i << std::endl;
		sensors.at(i)->init(getPosition(), getAbsoluteOrientation());
	}
}

void RobotAgentWorldModel::updateSensors(){
    for(unsigned int i = 0; i < sensors.size(); i++){
		sensors[i]->update(getPosition(), getAbsoluteOrientation());
	}
}

void RobotAgentWorldModel::resetSensors(){
    for(unsigned int i = 0; i < sensors.size(); i++){
		sensors[i]->reset();
	}
}

void RobotAgentWorldModel::displaySensorInformation(){
    for(unsigned int i = 0; i < sensors.size(); i++){
        sensors[i]->displaySensorInformation();
    }
}

void RobotAgentWorldModel::displaySensors(){
    std::deque<bool> displayed = std::deque<bool>(sensors[0]->getSensorCount(), false);
    for(unsigned int i = sensors.size()-1; i >= 1; i--){
        sensors[i]->displaySensor(gScreen, position, _agentAbsoluteOrientation, displayed, false);
    }
    sensors[0]->displaySensor(gScreen, position, _agentAbsoluteOrientation, displayed, true);
}

Point2d RobotAgentWorldModel::getPosition(){
    return position;
}

void RobotAgentWorldModel::setPosition(Point2d &pos){
    position = pos;
}

DefaultSensors* RobotAgentWorldModel::getDefaultSensors(){
	return defaultSensors;
}



