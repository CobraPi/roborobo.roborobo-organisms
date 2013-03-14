#ifndef SYMBRIONAGENTWORLDMODEL_H
#define SYMBRIONAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

class World;
class Organism;



class SymbrionAgentWorldModel : public RobotAgentWorldModel {
protected:
	double _distanceTraveled;
	int _stateLifetime; // nr of iterations in that state
public:
	SymbrionAgentWorldModel();
	virtual ~SymbrionAgentWorldModel();
	
	void setDistanceTraveled( double _distance );
	double getDistanceTraveled() const;
	
	int getStateLifetime() const;
	void setStateLifetime( int lifetime );
};

#endif

