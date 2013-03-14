/*
 *  EvolutionaryControlArchitecture.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 06-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EVOLUTIONARYCONTROLARCHITECTURE_H
#define EVOLUTIONARYCONTROLARCHITECTURE_H 

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "DifferentialControlArchitecture.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "lioutils/Mathlib.h"

using namespace std;

class EvolutionaryControlArchitecture : public DifferentialControlArchitecture
{
public:
	EvolutionaryControlArchitecture( RobotAgentWorldModel *__wm );
	~EvolutionaryControlArchitecture();
	
	virtual void step();
	virtual void reset();
	virtual ControllerPtr getNextCandidate() = 0;
	virtual void setCandidateFitness(ControllerPtr candidate, double scoredFitness) = 0;
	virtual ControllerPtr getActiveController();
	
	enum CalculationMethod {
		AVERAGE,
		CUMULATIVE
	};
	
	// the following variables are set by the properties files
	static int stepsPerCandidate;
	static CalculationMethod fitnessCalculation;
	const static int stepsPerFree = 80;
	const static int stepsPerRandomFree = 20;
	
protected:
	ControllerPtr candidate;
	
	const static bool debug = false;
	const static bool verbose = false;
	
	unsigned int phase;
	int stepsDone;
	double candidateFitness;
	
	// Override this to use a different fitness function
	// standard is distance moved
	virtual double calculateStepFitness();
	
	enum Phase {
		EVALUATING,
		FREEING_RANDOM,
		FREEING_MANUAL
	};
};


#endif
