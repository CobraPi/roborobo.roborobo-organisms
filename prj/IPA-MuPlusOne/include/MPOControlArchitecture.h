/*
 *  MPOControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */


#ifndef MPOCONTROLARCHITECTURE_H
#define MPOCONTROLARCHITECTURE_H 

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

using namespace std;

class MPOControlArchitecture : public EvolutionaryControlArchitecture
{
public:
	MPOControlArchitecture( RobotAgentWorldModel *__wm );
	~MPOControlArchitecture();
	
	virtual ControllerPtr getNextCandidate();
	virtual void setCandidateFitness(ControllerPtr candidate, double scoredFitness);

	// the following variables are set by the properties files
	static unsigned int mu;
	static double reEvaluationRate;
	static double crossoverRate;
	static double mutationRate;
        static int numberOfParents;
	
private:
	static int id;
	int myId;
	
	vector<ControllerPtr> hallOfFame;
	
	const static bool debug = false;
	const static bool verbose = false;

	int rankOf(ControllerPtr ctrl);
	
	enum Phase {
		EVALUATING,
		FREEING_RANDOM,
		FREEING_MANUAL
	};
};


#endif

