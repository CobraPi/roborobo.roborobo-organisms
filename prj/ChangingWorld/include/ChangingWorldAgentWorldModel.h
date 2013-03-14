
#ifndef ChangingWorldAGENTWORLDMODEL_H
#define ChangingWorldAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

#include "Organism.h"

class World;
class Organism;

class ChangingWorldAgentWorldModel : public RobotAgentWorldModel {	
public:
	// refer to super class to check what data / methods are available.

	ChangingWorldAgentWorldModel();
	virtual ~ChangingWorldAgentWorldModel();
    
    void setActive(bool _active);
	bool isActive() const;
	
	void setEnergyLevel(double _energyLevel);
	double getEnergyLevel() const;
	
	void setEnergyGained(double _energygained);
	double getEnergyGained() const;
    
    void setXStart(double x);
    double getXStart() const;
    
    void setYStart(double y);
    double getYStart() const;
    
private:
    bool _active; // agent stand still if not
    
    double _xStart;
    double _yStart;
    double _energyLevel;
	double _energyGained;
};


#endif


