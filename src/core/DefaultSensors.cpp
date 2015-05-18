#include "Sensor/DefaultSensors.h"
#include "World/EnergyPoint.h"
#include "Agents/RobotAgent.h"
#include "Utilities/SDL_gfxPrimitivesExtra.h"

DefaultSensors::DefaultSensors(){
//	_sensors = NULL;
	_ditchSensor = NULL;
	_energySensor = NULL;
	_sensorCount = 0;
	_floorSensor = 0;
}

DefaultSensors::~DefaultSensors(){
//	if ( _sensors != NULL )
//		delete[] _sensors;
	if ( _ditchSensor != NULL )
		delete[] _ditchSensor;
	if ( _energySensor != NULL )
		delete[] _energySensor;
}

std::vector<std::vector<double> > DefaultSensors::getSensors(){
	return _sensors;
}

void DefaultSensors::init(Point2d position, double orientation){
	_sensorCount = 0;

	//count sensors
	for (int x = 0; x != gAgentWidth; x++){ // image is analysed a first time to count the number of sensors (faster than dynamically re-allocating array size for every new sensor)
		for (int y = 0; y != gAgentHeight; y++) {
			Uint32 pixel = getPixel32(gAgentSpecsImage, x, y);
			if (pixel != SDL_MapRGB(gAgentSpecsImage->format, 0xFF, 0xFF, 0xFF)) {
				_sensorCount++;
            }
        }
    }
//	_sensors = new double[_sensorCount][7]; // see header for details.
	_ditchSensor = new double[_sensorCount];
	_energySensor = new double[_sensorCount][2];

	for (int i = 0; i < _sensorCount; i++) {
        std::vector<double> _sensor(7);
		_sensor[0] = -1;
		// 1 - 4 set below
		_sensor[5] = -1;
		_sensor[6] = -1;
        
        _sensors.push_back(_sensor);

		_ditchSensor[i] = -1;
        _energySensor[i][0] = -1;
        _energySensor[i][1] = -1;
    }
	//int sensorIt = 0;

	//register sensors
	for (int x = 0; x != gAgentWidth; x++){
		for (int y = 0; y != gAgentHeight; y++) {
			Uint32 pixel = getPixel32(gAgentSpecsImage, x, y);
			if (pixel != SDL_MapRGB(gAgentSpecsImage->format, 0xFF, 0xFF, 0xFF)) {
				// sensor found, register sensor.

				Uint8 r, g, b;
				SDL_GetRGB(pixel, gAgentSpecsImage->format, &r, &g, &b);

				if (_sensors[r][0] != -1) {
					std::cout << "[ERROR] robot sensor id already in use -- check agent specification image." << std::endl;
					exit(-1);
				}

				if (r >= _sensorCount) {
					std::cout << "[ERROR] robot sensor id is not permitted (must be defined btw 0 and " << (_sensorCount - 1) << ", got: " << r << ") -- check agent specification image." << std::endl;
					exit(-1);
				}


				_sensors[r][0] = r; // no. sensor

				// sensor origini point location wrt. agent center
				_sensors[r][1] = sqrt((x - gAgentWidth / 2) * (x - gAgentWidth / 2) + (y - gAgentHeight / 2) * (y - gAgentHeight / 2)); // length
				double angleCosinus = ((x - (gAgentWidth / 2)) / _sensors[r][1]);
				double angleSinus = ((y - (gAgentHeight / 2)) / _sensors[r][1]);
				if (angleSinus >= 0)
					_sensors[r][2] = acos(angleCosinus) + M_PI / 2; // angle (in radian)
				else
					_sensors[r][2] = -acos(angleCosinus) + M_PI / 2 + M_PI * 2; // angle (in radian)

				// sensor target point location wrt. agent center -- sensor target angle is (green+blue) component values
				double angle = g + b - 90; // note: '-90deg' is due to image definition convention (in image, 0° means front of agent, which is upward -- while 0° in simulation means facing right)
				double xTarget = (x - gAgentWidth / 2) + cos(angle * M_PI / 180) * gSensorRange;
				double yTarget = (y - gAgentHeight / 2) + sin(angle * M_PI / 180) * gSensorRange;
				_sensors[r][3] = sqrt(xTarget * xTarget + yTarget * yTarget); // length (**from agent center**)
				angleCosinus = xTarget / _sensors[r][3];
				angleSinus = yTarget / _sensors[r][3];
				if (angleSinus >= 0)
					_sensors[r][4] = acos(angleCosinus) + M_PI / 2; // angle (in radian) wrt. agent center
				else
					_sensors[r][4] = -acos(angleCosinus) + M_PI / 2 + M_PI * 2;
				r++;
			}
		}
	}
}


void DefaultSensors::update(Point2d position, double orientation){
	// update sensors
	for (int i = 0; i < _sensorCount; i++) {
		// Warning: the following is repeated in the show method because coordinates are not stored, but are needed to display the sensor rays.
		double x1 = (position.x + _sensors[i][1] * cos(_sensors[i][2] + orientation * M_PI / 180));
		double y1 = (position.y + _sensors[i][1] * sin(_sensors[i][2] + orientation * M_PI / 180));
		double x2 = (position.x + _sensors[i][3] * cos(_sensors[i][4] + orientation * M_PI / 180));
		double y2 = (position.y + _sensors[i][3] * sin(_sensors[i][4] + orientation * M_PI / 180));

		// cast sensor ray.
		_sensors[i][5] = castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object. -- not used here.
		Uint8 r, g, b;
		Uint32 pixel = getPixel32(gEnvironmentImage, x2, y2);
		SDL_GetRGB(pixel, gEnvironmentImage->format, &r, &g, &b);
		_sensors[i][6] = (r << 16)+(g << 8) + b; // R=objects, B=agents, can only be one agent OR one object in each location.

		if(gEnergyMode) {
			// Cast another sensor on the energy map to detect energy points
            ResourceFactory<EnergyPoint>::ResourceFactoryPtr factory = ResourceFactory<EnergyPoint>::getInstance();

            
			double xx1 = (position.x + _sensors[i][1] * cos(_sensors[i][2] + orientation * M_PI / 180));
			double yy1 = (position.y + _sensors[i][1] * sin(_sensors[i][2] + orientation * M_PI / 180));
			double xx2 = (position.x + _sensors[i][3] * cos(_sensors[i][4] + orientation * M_PI / 180));
			double yy2 = (position.y + _sensors[i][3] * sin(_sensors[i][4] + orientation * M_PI / 180));
			_energySensor[i][0] = factory->castRay(xx1, yy1, &xx2, &yy2, getSensorMaximumDistanceValue(i));

            Uint8 r, g, b;
            factory->getRGB(xx2, yy2, &r, &g, &b);
            if(r != 0xff){
                _energySensor[i][1] = r; // R=level of energy
            }else{
                _energySensor[i][1] = 0; // white = no energy
            }
        }

	} //(old: (r<<16)+(g<<8)+b;)


	Uint8 r, g, b;
	Uint32 pixel = getPixel32(gZoneImage, position.x, position.y);
	SDL_GetRGB(pixel, gZoneImage->format, &r, &g, &b);
	_floorSensor = r;
}


void DefaultSensors::reset(){
	for (int i = 0; i != _sensorCount; i++) { // initialize sensor values to max range, no contact
		_sensors[i][5] = gSensorRange; // range: max
		_sensors[i][6] = 0; // type:  none
	}
	_floorSensor = 0; // floor sensor value (taken from gZoneImage)
}


void DefaultSensors::displaySensor(SDL_Surface *screen, Point2d position, double orientation, std::deque<bool> &displayed, bool force){
	// * show sensors
	for (int i = 0; i < _sensorCount; i++) {
		// Warning: the following is a repetition of code already in the move method (sensor ray casting) in order to display it (coordinates are not stored)
		double x1 = (position.x + _sensors[i][1] * cos(_sensors[i][2] + orientation * M_PI / 180));
		double y1 = (position.y + _sensors[i][1] * sin(_sensors[i][2] + orientation * M_PI / 180));
		double x2 = (position.x + _sensors[i][3] * cos(_sensors[i][4] + orientation * M_PI / 180));
		double y2 = (position.y + _sensors[i][3] * sin(_sensors[i][4] + orientation * M_PI / 180));

		// sensor ray casting is also performed in the move method -- this time we dont store data (already done). -- this one is only used to *display* the ray.
		/*_sensors[i][5] = */
		castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object.

		// display on screen
		if (!displayed[i] && _sensors[i][5] < getSensorMaximumDistanceValue(i) - 1) {//gSensorRange-1 )
            displayed[i] = true;
			traceRayRGBA(screen, int(x1 + 0.5) - gCamera.x, int(y1 + 0.5) - gCamera.y, int(x2 + 0.5) - gCamera.x, int(y2 + 0.5) - gCamera.y, 255, 0, 0, 255);
		} else if (gEnergyMode && !displayed[i] && _energySensor[i][0] < getSensorMaximumDistanceValue(i) - 1) {
            displayed[i] = true;
			traceRayRGBA(screen, int(x1 + 0.5) - gCamera.x, int(y1 + 0.5) - gCamera.y, int(x2 + 0.5) - gCamera.x, int(y2 + 0.5) - gCamera.y, 0, 255, 0, 255);
        } else if (!displayed[i] && force) {
            displayed[i] = true;
			traceRayRGBA(screen, int(x1 + 0.5) - gCamera.x, int(y1 + 0.5) - gCamera.y, int(x2 + 0.5) - gCamera.x, int(y2 + 0.5) - gCamera.y, 0, 0, 255, 255);
        }
	}
}


double DefaultSensors::getSensorDistanceValue( int i ){
	// return sensor range (equal to maximum range if no obstacle)
	return _sensors[i][5];
}


double DefaultSensors::getSensorTypeValue( int i ){
	// return type of obstacle hit by sensor ray (if any)
	return _sensors[i][6];
}


double DefaultSensors::getSensorMaximumDistanceValue( int i ){
	// return sensor maximum range
	return gSensorRange; // BEWARE: sensor[i][3] is distance btw the end point
	//of the sensor and the **center of the agent**
}


int DefaultSensors::getFloorSensor(){
	// return floor sensor value
	return _floorSensor;
}


double DefaultSensors::getDitchDistanceValue( int i ){
	// return distance sensor for ditches
	return _ditchSensor[i];
}

double DefaultSensors::getEnergyDistanceValue( int i)
{
    return _energySensor[i][0];
}

double DefaultSensors::getEnergyLevelValue( int i)
{
    return _energySensor[i][1];
}

void DefaultSensors::displaySensorInformation(){
	for(int i = 0; i < _sensorCount; i++){
		std::cout << getSensorDistanceValue(i) << "/" << getSensorMaximumDistanceValue(i) << " ";
	}
}

std::vector<RobotAgentPtr> DefaultSensors::getNearRobots(RobotAgent *agent){
	std::vector<RobotAgentPtr> agents;
	for(unsigned int i = 0; i < _sensorCount; i++){
        if(_sensors[i][6] >= 1000 && _sensors[i][6] <= 2000){
            RobotAgentPtr other = gWorld->getAgent(_sensors[i][6]-1000);
            if(!gUseOrganisms || !agent->isPartOfSameOrganism(other)){
				agents.push_back(gWorld->getAgent(_sensors[i][6]-1000));
			}
        }
	}

	return agents;
}

int DefaultSensors::getSensorCount(){
	return _sensorCount;
}
