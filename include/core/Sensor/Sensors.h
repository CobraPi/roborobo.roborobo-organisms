/**
 * Sensors.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Job Bakker
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"


/**
 * Abstract class Sensors
 *
 * You can implement this class using your own nice and shiny sensor type. It
 * will be called accordingly.
 */
class Sensors{
public:
	Sensors();
	virtual ~Sensors();


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
	virtual void init(Point2d *position, double orientation) = 0;


	/**
	 * Update your sensor values. Required.
	 */
	virtual void update(Point2d *position, double orientation) = 0;


	/**
	 * Resets all sensors
	 */
	virtual void reset() = 0;


	/**
	 * Used for displaying the sensors on the screen,
	 * when not using the batch mode. If you want to use
	 * batch mode, or do not want to show the sensor on the screen,
	 * implementing this class is not necessary per sè. (however, it
	 * would be nice and preferable)
	 *
	 * @param screen
	 * 		The screen to paint the sensors on
	 */
	virtual void displaySensors(SDL_Surface *screen, Point2d *position, double orientation) = 0;


	/**
	 * Returns the total number of sensors
	 *
	 * @return
	 * 		The number of sensors
	 */
	virtual int getSensorCount() = 0;


	/**
	 * Displays sensor information. Typically the min and max range of the
	 * sensors.
	 */
	virtual void displaySensorInformation() = 0;

};


#endif /* SENSORS_H_ */
