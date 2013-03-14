

#ifndef SCALEPOWERCONTROLARCHITECTURE_H
#define SCALEPOWERCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class ScalePowerControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	ScalePowerControlArchitecture(RobotAgentWorldModel *__wm);
	virtual ~ScalePowerControlArchitecture();

	virtual ControllerPtr createRandomGenome();
	virtual double calculateStepFitness();
};


#endif

