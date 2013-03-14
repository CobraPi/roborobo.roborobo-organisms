

#ifndef ChangingWorldCONTROLARCHITECTURE_H
#define ChangingWorldCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class ChangingWorldControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	ChangingWorldControlArchitecture(RobotAgentWorldModel *__wm);
	virtual ~ChangingWorldControlArchitecture();

	virtual ControllerPtr createRandomGenome();
	virtual double calculateStepFitness();
	
	static double migrationRate;
	static double eliteSize;
	static double bestRate;
	static int migrationType;
	static int topologyType;
	
private:
	bool isInVector(ControllerPtr cont, vector<ControllerPtr> * theVector);
};


#endif

