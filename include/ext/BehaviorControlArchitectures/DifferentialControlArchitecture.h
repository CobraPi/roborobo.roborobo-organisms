/*
 *  DifferentialControlArchitecture.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DIFFERENTIALCONTROLARCHITECTURE_H
#define DIFFERENTIALCONTROLARCHITECTURE_H 

/* To determine the number of pixels per meter, we take our only point of reference: the epuck robot. This
 * is known to be approximately 10 cm in diameter, and is 32 pixels wide on our screen. This means that 1
 * meter is 320 pixels.
 * 
 * To determine the number of ticks per second, we use the "normal" display mode as reference: 60 fps.
 */

#define TICKSPERSECOND 60
#define PIXELSPERMETER 320

#include <iostream>
#include <vector>
#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utils/Rand.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"

#include "WorldModels/RobotAgentWorldModel.h"

using namespace std;

class DifferentialControlArchitecture : public BehaviorControlArchitecture
{
private:
	static const double maxSpeed;
	
public:
	//Initializes the variables
	DifferentialControlArchitecture( RobotAgentWorldModel *__wm );
	~DifferentialControlArchitecture();
	
	virtual void reset() = 0;
	virtual void step() = 0;
	
	void setSpeed(double left, double right);
	virtual vector<double>* getSensorValues();
	
};


#endif

