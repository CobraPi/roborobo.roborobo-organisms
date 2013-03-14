/*
 *  HiveMindControlArchitecture.h
 *  roborobo-online
 *
 */


#ifndef HIVEMINDCONTROLARCHITECTURE_H
#define HIVEMINDCONTROLARCHITECTURE_H 

#include <iostream>

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

using namespace std;

class HiveMindControlArchitecture : public PopulationControlArchitecture
{
public:
	HiveMindControlArchitecture( RobotAgentWorldModel *__wm );
	~HiveMindControlArchitecture();

	virtual ControllerPtr createRandomGenome();
};


#endif

