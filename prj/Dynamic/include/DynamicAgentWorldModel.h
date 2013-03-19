
#ifndef DYNAMICAGENTWORLDMODEL_H
#define DYNAMICAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

#include "Organism.h"
#include "World/TypedEnergyPoint.h"
#include "Sensor/ResourceSensor.h"

class World;
class Organism;

class DynamicAgentWorldModel : public RobotAgentWorldModel {
private:
	bool _active; // agent stand still if not

	double _angleToClosestEnergyPoint;
	double _distanceToClosestEnergyPoint;
    
    ResourceSensor<TypedEnergyPoint> *typedEnergySensor;
public:
	unsigned int swarmTime;
	unsigned int organismTime;
	int penalty;
    Point2d start;

	// refer to super class to check what data / methods are available.

	DynamicAgentWorldModel();
	virtual ~DynamicAgentWorldModel();

	void setActive(bool _active);
	bool isActive() const;

	void setAngleToClosestEnergyPoint(double __value);
	double getAngleToClosestEnergyPoint();

	void setDistanceToClosestEnergyPoint(double __value);
	double getDistanceToClosestEnergyPoint();
    
    ResourceSensor<TypedEnergyPoint> *getTypedEnergySensor();
};


#endif


