

#ifndef DYNAMICCONTROLARCHITECTURE_H
#define DYNAMICCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/NewscastAgent.h"
#include "newscast/NewscastAgentGroup.h"
#include "newscast/HiveMindInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class DynamicControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	DynamicControlArchitecture(RobotAgentWorldModel *__wm);
	virtual ~DynamicControlArchitecture();
	
	virtual ControllerPtr createRandomGenome();

	virtual double calculateStepFitness();
    
    static int cacheSize;
	static int itemTTL;
};


#endif

