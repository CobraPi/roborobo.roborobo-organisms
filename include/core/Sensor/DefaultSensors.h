/*
 * DefaultSensors.h
 *
 *  Created on: Mar 13, 2013
 *      Author: job
 */

#ifndef DEFAULTSENSORS_H_
#define DEFAULTSENSORS_H_

#include "RoboroboMain/roborobo.h"

#include "Sensor/Sensor.h"

class DefaultSensors : public Sensor {
private:
	// description and values of sensors
	/// id sensor(R),sensor_origin_norm,sensor_origin_angle,
	/// sensor_target_distance_from_agent_center(!),sensor_target_angle,
	/// "current value", object Id.
	double (*_sensors)[7];
	/// result is the **red** value from gZoneImage by raycasting the same
	/// sensor as the normal sensors
	double (*_ditchSensor);
	double (*_energySensor)[2];
	int _sensorCount;
	/// result is the **red** value from Zonemage at the center of the agent
	int _floorSensor;

public:
	DefaultSensors();

	virtual ~DefaultSensors();

	/**
	 * Initializes the sensors. Required.
	 *
	 * Typically, sensors are read from a file and then initialized (for example
	 * the standard object detection sensors). This will initialize for example
	 * the angle and distance of the sensor, the number of sensors, the color of
	 * the sensors etc. This kind of initialization needs to be done in the init
	 * method, and is therefore required.
	 *
	 * @param _wm
	 */
	void init(Point2d position, double orientation);


	/**
	 * Update your sensor values. Required.
	 */
	void update(Point2d position, double orientation);


	/**
	 * Resets all sensors
	 */
	void reset();


	/**
	 * Used for displaying the sensors on the screen,
	 * when not using the batch mode. If you want to use
	 * batch mode, or do not want to show the sensor on the screen,
	 * implementing thierror in C++ - Stack Overflow
stackoverflow.com/questions/.../ds class is not necessary per sè. (however, it
	 * would be nice and preferable)
	 *
	 * @param screen
	 * 		The screen to paint the sensors on
	 */
    void displaySensor(SDL_Surface *screen, Point2d position, double orientation, std::deque<bool> &displayed, bool force);


	/**
	 * return sensor range (equal to maximum range if no obstacle)
	 *
	 * @param i
	 * @return
	 */
	double getSensorDistanceValue( int i );


	/**
	 * return type of obstacle hit by sens
RobotAgentWorldModel::~RobotAgentWorldModelor ray (if any)
	 *
	 * @param i
	 * @return
	 */
	double getSensorTypeValue( int i );



	/**
	 * return sensor maximum range
	 *
	 * @param i
	 * 		The sensor number
	 * @return
	 * 		The maximum sensor range of sensor i
	 */
	double getSensorMaximumDistanceValue( int i );


	/**
	 * return floor sensor value
	 *
	 * @return
	 * 		The floor sensor value
	 */
	int getFloorSensor();


	/**
	 * return distance sensor for ditches
	 *
	 * @param i
	 * 		The ditch sensor number
	 * @return
	 * 		The distance sensor value for sensor i
	 */
	double getDitchDistanceValue( int i );


	/**
	 *	return the energy distance value
	 *
	 * @param i
	 * 		The energy distance sensor number
	 * @return
	 * 		The energy distance sensor value for sensor i
	 */
	double getEnergyDistanceValue( int i );

	/**
	 *	return the energy level value
	 *
	 * @param i
	 * 		The energy level sensor number
	 * @return
	 * 		The energy level sensor value for sensor i
	 */
	double getEnergyLevelValue( int i);

	/**
	 * Displays sensor information. Typically the min and max range of the
	 * sensors.
	 */
	void displaySensorInformation();


	double** getSensors();

	std::vector<RobotAgentPtr> getNearRobots(RobotAgent *agent);

	int getSensorCount();
};


#endif /* DEFAULTSENSORS_H_ */
