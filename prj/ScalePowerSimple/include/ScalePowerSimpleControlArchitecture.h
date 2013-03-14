

#ifndef ScalePowerSimpleCONTROLARCHITECTURE_H
#define ScalePowerSimpleCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class ScalePowerSimpleControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	ScalePowerSimpleControlArchitecture(RobotAgentWorldModel *__wm);
	~ScalePowerSimpleControlArchitecture();

	virtual ControllerPtr createRandomGenome();
	virtual double calculateStepFitness();
};


#endif

