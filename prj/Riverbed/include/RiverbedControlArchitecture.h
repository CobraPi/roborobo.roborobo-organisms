

#ifndef RIVERBEDCONTROLARCHITECTURE_H
#define RIVERBEDCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class RiverbedControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	RiverbedControlArchitecture(RobotAgentWorldModel *__wm);
	virtual ~RiverbedControlArchitecture();
	
	virtual ControllerPtr createRandomGenome();

	virtual double calculateStepFitness();
};


#endif

