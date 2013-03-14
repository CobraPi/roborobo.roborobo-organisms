/*
 *  PopulationControlArchitecture.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef POPULATIONCONTROLARCHITECTURE_H
#define POPULATIONCONTROLARCHITECTURE_H 

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "lioutils/Mathlib.h"

#include "newscast/PopulationMember.h"
#include "newscast/NewsItem.h"

using namespace std;

class PopulationControlArchitecture  : public EvolutionaryControlArchitecture
{
public:
	PopulationControlArchitecture( RobotAgentWorldModel *__wm );
	~PopulationControlArchitecture();
	
	virtual ControllerPtr getNextCandidate();
	virtual void setCandidateFitness(ControllerPtr candidate, double scoredFitness);
	// This function is called by getNextCandidate(), should be overridden
	virtual ControllerPtr createRandomGenome() = 0;
	
	void setPopulation(PopulationMember *pop);
	
	// the following variables are set by the properties files
	static unsigned int mu; // TODO: let this actually be set!
	static double reEvaluationRate;
	static double crossoverRate;
	static double mutationRate;
	static int numberOfParents;
	
protected:
	const static bool debug = false;
	const static bool verbose = false;
	vector<ControllerPtr> activeSolutions;

	PopulationMember *population;
        
	int currentTime;
	
	ControllerPtr selectParentByBinaryTournament(ControllerPtr exclude);
	ControllerPtr selectParentsByBinaryTournament(vector<ControllerPtr> *exclude);
};


#endif
