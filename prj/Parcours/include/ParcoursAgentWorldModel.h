
#ifndef ParcoursAGENTWORLDMODEL_H
#define ParcoursAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

#include "Organism.h"

class World;
class Organism;

class ParcoursAgentWorldModel : public RobotAgentWorldModel {	
public:
	// refer to super class to check what data / methods are available.

	ParcoursAgentWorldModel();
	virtual ~ParcoursAgentWorldModel();
	
	unsigned int swarmTime;
	unsigned int organismTime;
	int penalty;
	bool ditchCheck;
    double xStart;
};


#endif


