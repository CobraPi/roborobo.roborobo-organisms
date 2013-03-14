
#ifndef RIVERBEDAGENTWORLDMODEL_H
#define RIVERBEDAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

#include "Organism.h"

class World;
class Organism;

class RiverbedAgentWorldModel : public RobotAgentWorldModel {
private:
	bool _active; // agent stand still if not

	double _energyLevel;
	double _deltaEnergy;
	double _energyGained;

	double _angleToClosestEnergyPoint;
	double _distanceToClosestEnergyPoint;
	
public:
	// refer to super class to check what data / methods are available.

	RiverbedAgentWorldModel();
	virtual ~RiverbedAgentWorldModel();

	void setActive(bool _active);
	bool isActive() const;
	
	void setDeltaEnergy(double __deltaEnergy);
	double getDeltaEnergy() const;
	
	void setEnergyLevel(double __energyLevel);
	double getEnergyLevel() const;
	
	void setEnergyGained(double __energygained);
	double getEnergyGained() const;

	void setAngleToClosestEnergyPoint(double __value);
	double getAngleToClosestEnergyPoint();

	void setDistanceToClosestEnergyPoint(double __value);
	double getDistanceToClosestEnergyPoint();
};


#endif


