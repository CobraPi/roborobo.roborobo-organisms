/**
 * PuckSensors.h
 *
 * Refactoring of RangeSensor by Nikita
 *
 *  Created on: Mar 6, 2013
 *      Author: Job Bakker
 */

#ifndef PUCKSENSORS_H_
#define PUCKSENSORS_H_

#include "Sensor/Sensors.h"
#include "RoboroboMain/common.h"
#include "Utilities/SDL_gfxPrimitivesExtra.h"

struct SensorRay;

class PuckSensors : public Sensors{
public:
	/**
	 *  Sensor id. I don't believe it's used anywhere.
	 */
	int _id;


	/**
	 *  Sensor ray goes from origin to target -- angles and distances to those
	 *  are measured from the center of the agent
	 */
	double _originDistance;
	double _originAngle;
	double _targetDistance;
	double _targetAngle;


	/** Colors are numbered from 0 to 15 -- green component corresponds to index
	 *  (hate to rephrase certain celebrity, but 16 colors should be enough).
	 */
	static const int MAX_PUCK_COLORS = 16;
	int _puckRanges[MAX_PUCK_COLORS];
	int _obstacleRange;
	int _obstacleId;


	/**
	 * x1, y1, x2, y2 and maxRange.
	 */
	SensorRay _sensorRay;


	PuckSensors(int id, double originDistance, double originAngle,
		double targetDistance, double targetAngle, double maxSensorDistance);


	~PuckSensors();


	/**
	 * Initializes the sensors. See the interface method description for a more
	 * thorough explanation.
	 */
	void init(Point2d *position, double orientation);


	/**
	 * Updates the sensors. When this method is called,
	 */
	void update(Point2d *position, double orientation);


	/**
	 * Resets all sensors to the max_range value
	 *
	 * @param _wm
	 * 		The world model to be reset
	 */
	void reset();


	int getSensorCount();


	/**
	 * Casts the sensors on the screen when the batch mode is not used.
	 *
	 * @param screen
	 * 		The surface to draw on
	 */
	void displaySensors(SDL_Surface *screen, Point2d *position, double orientation);


	/**
	 * Displays sensor information. Typically the min and max range of the
	 * sensors.
	 */
	virtual void displaySensorInformation();


	void getNearRobots(std::vector<RobotAgentPtr> *agents, RobotAgent *agent);


	void getOldSensorData(double* sensorData) {
		sensorData[0] = (double)_id;
		sensorData[1] = _originDistance;
		sensorData[2] = _originAngle;
		sensorData[3] = _targetDistance;
		sensorData[4] = _targetAngle;
		/* Range to obstacle [5] and obstacle id [6] are updated elsewhere. */
	}


	double getNormObstacleRange();


	double getNormPuckRange(int i);
};


#endif /* PUCKSENSORS_H_ */
