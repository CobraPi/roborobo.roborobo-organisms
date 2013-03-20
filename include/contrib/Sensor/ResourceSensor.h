//
//  ResourceSensor.h
//  roborobo
//
//  Created by Berend Weel on 3/19/13.
//
//

#ifndef roborobo_ResourceSensor_h
#define roborobo_ResourceSensor_h

#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>

#include "Sensor/Sensor.h"
#include "World/ResourceFactory.h"

/**
 * \class ResourceSensor
 *
 * Template class that implements a sensor that senses Resources that are managed by a ResourceFactory.
 * \sa Resource \sa ResourceFactory
 *
 * This sensor is able to sense two things:
 *  - The range at which something has been sensed
 *  - The id of the object that has been sensed. This is determined by the red value of the Resource color.
 *
 *
 */
template<class Resource>
class ResourceSensor : public Sensor {
public:
    ResourceSensor(){
        factory = ResourceFactory<Resource>::getInstance();
    }
    
    ~ResourceSensor(){
        factory.reset();
        _sensors.clear();
    }
    
    
	/**
	 * Initializes the sensors.
	 *
     * This implementation uses the default gAgentSpecsImage
	 *
	 */
	virtual void init(Point2d position, double orientation) {       
        for (int x = 0; x != gAgentWidth; x++){
            for (int y = 0; y != gAgentHeight; y++) {
                Uint32 pixel = getPixel32(gAgentSpecsImage, x, y);
                if (pixel != SDL_MapRGB(gAgentSpecsImage->format, 0xFF, 0xFF, 0xFF)) {
                    // sensor found, register sensor.
                    
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, gAgentSpecsImage->format, &r, &g, &b);
                    
                    if (r < _sensors.size() && !_sensors.is_null(r)) {
                        std::cout << "[ERROR] robot sensor id already in use -- check agent specification image." << std::endl;
                        assert(r >= _sensors.size() || ( r < _sensors.size() && _sensors.is_null(r)));
                    }

                    _sensors.push_back(new SensorData);
                    _sensors[r].id = r; // no. sensor
                    _sensors[r].value = -1;
                    _sensors[r].objectId = -1;
                    
                    // sensor origini point location wrt. agent center
                    _sensors[r].origin = sqrt((x - gAgentWidth / 2) * (x - gAgentWidth / 2) + (y - gAgentHeight / 2) * (y - gAgentHeight / 2)); // length
                    double angleCosinus = ((x - (gAgentWidth / 2)) / _sensors[r].origin);
                    double angleSinus = ((y - (gAgentHeight / 2)) / _sensors[r].origin);
                    if (angleSinus >= 0)
                        _sensors[r].originAngle = acos(angleCosinus) + M_PI / 2; // angle (in radian)
                    else
                        _sensors[r].originAngle = -acos(angleCosinus) + M_PI / 2 + M_PI * 2; // angle (in radian)
                    
                    // sensor target point location wrt. agent center -- sensor target angle is (green+blue) component values
                    double angle = g + b - 90; // note: '-90deg' is due to image definition convention (in image, 0° means front of agent, which is upward -- while 0° in simulation means facing right)
                    double xTarget = (x - gAgentWidth / 2) + cos(angle * M_PI / 180) * gSensorRange;
                    double yTarget = (y - gAgentHeight / 2) + sin(angle * M_PI / 180) * gSensorRange;
                    _sensors[r].length = sqrt(xTarget * xTarget + yTarget * yTarget); // length (**from agent center**)
                    angleCosinus = xTarget / _sensors[r].length;
                    angleSinus = yTarget / _sensors[r].length;
                    if (angleSinus >= 0)
                        _sensors[r].angle = acos(angleCosinus) + M_PI / 2; // angle (in radian) wrt. agent center
                    else
                        _sensors[r].angle = -acos(angleCosinus) + M_PI / 2 + M_PI * 2;
                    r++;
                }
            }
        }
    }
    
    
	/**
	 * Updates the sensors.
	 */
	virtual void update(Point2d position, double orientation){
        for (int i = 0; i < _sensors.size() ; i++) {
            // cast sensor ray.
            double x1, y1, x2, y2;
            getSensorPositions(position, orientation,i, x1,y1,x2,y2);
            
            _sensors[i].value = factory->castRay(x1, y1, &x2, &y2, getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object.
            Uint8 r, g, b;
            factory->getRGB(x2,y2,&r,&g,&b);
            
            if(r != 0xff){
                _sensors[i].objectId = r; // R=objectId
            }else{
                _sensors[i].objectId = 0; // white = no energy
            }
        }
    }
    
    
	/**
	 * Resets all sensors
	 */
	virtual void reset(){
        for (int i = 0; i != _sensors.size(); i++) { // initialize sensor values to max range, no contact
            _sensors[i].value = gSensorRange;
            _sensors[i].objectId = 0;
        }
    }
    
	/**
	 * Used for displaying the sensors on the screen,
	 * when not using the batch mode. 
	 *
	 * @param screen
	 * 		The screen to paint the sensors on
     *
     * @param position
     *      The positoin of the robot
	 */
	virtual void displaySensor(SDL_Surface *screen, Point2d position, double orientation, std::deque<bool> &displayed, bool force){
        for (int i = 0; i < _sensors.size() ; i++) {
            // cast sensor ray.
            double x1, y1, x2, y2;
            getSensorPositions(position, orientation,i, x1,y1,x2,y2);
            factory->castRay(x1, y1, &x2, &y2, getSensorMaximumDistanceValue(i));
            
            if(_sensors[i].value < getSensorMaximumDistanceValue(i) - 1){
                displayed[i] = true;
                traceRayRGBA(screen, int(x1 + 0.5) - gCamera.x, int(y1 + 0.5) - gCamera.y, int(x2 + 0.5) - gCamera.x, int(y2 + 0.5) - gCamera.y, 255, 0, 0, 255);
            }else if(!displayed[i] && force){
                displayed[i] = true;
                traceRayRGBA(screen, int(x1 + 0.5) - gCamera.x, int(y1 + 0.5) - gCamera.y, int(x2 + 0.5) - gCamera.x, int(y2 + 0.5) - gCamera.y, 0, 0, 0, 255);
            }
        }
    }
    
    
	/**
	 * Returns the total number of sensors
	 *
	 * @return
	 * 		The number of sensors
	 */
	virtual int getSensorCount(){
        return _sensors.size();
    }
    
	/**
	 * Displays sensor information. Typically the min and max range of the
	 * sensors.
	 */
	virtual void displaySensorInformation(){
        
    }
    /**
	 * Returns the sensor range (equal to maximum range if no obstacle)
	 *
	 * @param i
	 * @return the range that was sensed
	 */
	double getSensorValue( int i ){
        return ((SensorData)_sensors[i]).value;
    }
    
    
	/**
	 * Returns the id of the obstacle hit by sensor ray (if any)
	 *
	 * @param i
	 * @return The object id
	 */
	double getSensorTypeValue( int i ){
        return ((SensorData)_sensors[i]).objectId;
    }
    
	/**
	 * return sensor maximum range
	 *
	 * @param i
	 * 		The sensor number
	 * @return
	 * 		The maximum sensor range of sensor i
	 */
	double getSensorMaximumDistanceValue( int i ){
        // TODO: internalize this value?
        return gSensorRange;
    }
    
private:
    
    /**
     * Calculates the start and end positions of the sensors based on the position and orientation of the robot.
     *
     * @param position The robots position
     * @param orientation The robots orientation (in radians)
     * @param sensorId The id of the sensor
     * @param x1 Destination for the x-coordinate of the sensor origin
     * @param y1 Destination for the y-coordinate of the sensor origin
     * @param x2 Destination for the x-coordinate of the sensor target
     * @param y2 Destination for the y-coordinate of the sensor target
     */
    void getSensorPositions(Point2d position, double orientation, int sensorId, double &x1, double &y1, double &x2, double &y2){
        // Warning: the following is repeated in the show method because coordinates are not stored, but are needed to display the sensor rays.
        x1 = (position.x + _sensors[sensorId].origin * cos(_sensors[sensorId].originAngle + orientation * M_PI / 180));
        y1 = (position.y + _sensors[sensorId].origin * sin(_sensors[sensorId].originAngle + orientation * M_PI / 180));
        x2 = (position.x + _sensors[sensorId].length * cos(_sensors[sensorId].angle + orientation * M_PI / 180));
        y2 = (position.y + _sensors[sensorId].length * sin(_sensors[sensorId].angle + orientation * M_PI / 180));
    }
    
    boost::ptr_vector< boost::nullable<SensorData> > _sensors;
    typename ResourceFactory<Resource>::ResourceFactoryPtr factory;
};

#endif
