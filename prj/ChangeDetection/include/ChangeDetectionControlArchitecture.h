

#ifndef ChangeDetectionCONTROLARCHITECTURE_H
#define ChangeDetectionCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "multikulti/MultiKultiInstance.h"

#include "WorldModels/RobotAgentWorldModel.h"

class ChangeDetectionControlArchitecture : public PopulationControlArchitecture {
public:
	//Initializes the variables
	ChangeDetectionControlArchitecture(RobotAgentWorldModel *__wm);
	virtual ~ChangeDetectionControlArchitecture();

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

