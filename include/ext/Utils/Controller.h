/*
 *  Controller.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "RoboroboMain/roborobo.h"
#include "lioutils/lio_global.h"
#include "WorldModels/RobotAgentWorldModel.h"
#include "Organism.h"

using namespace std;

class Controller;
typedef boost::shared_ptr<Controller> ControllerPtr;

class Controller {
public:
	Controller();
	Controller(vector<double> _mutationStepSizes);
	virtual ~Controller();
	
	static bool compare(ControllerPtr one, ControllerPtr other);
	
	// This is where the controller actually controls the robot
	// THESE FUNCTIONS MUST BE IMPLEMENTED!
	virtual void step(double &left, double &right) = 0;
	virtual void reset() = 0;
	virtual ControllerPtr Clone() = 0;
	virtual ControllerPtr crossOverWith(ControllerPtr partner) = 0;
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents) = 0;
	virtual void mutate() = 0;
	
	virtual void joinedOrganism(Organism *organism);

	virtual void setWorldModel(RobotAgentWorldModel *__wm);
	virtual double GetOriginalFitness();
	
	virtual double GetFitness();
	virtual void SetFitness(double fitness);
	
	virtual double GetMutationStepSize(int i);
	virtual void SetMutationStepSize(int i, double stepSize);
	
	virtual string ToString();
	virtual string ToShortString();
	
	static double initialMutationStepSize;
	static double controllerMutationStepSizeMinimum;

protected:
	virtual void SetOriginalFitness(double originalFitness);

	vector<double> mutationStepSizes;
	
	static int nextId;
	int id;
	double fitness, originalFitness;
	
	RobotAgentWorldModel *_wm;
};

#endif /* CONTROLLER_H_ */
