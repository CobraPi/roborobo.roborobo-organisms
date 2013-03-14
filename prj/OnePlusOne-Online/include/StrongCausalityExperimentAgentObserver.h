/*
 *  StrongCausalityExperimentGeneticsControlArchitecture.h
 *  roborobo
 *
 *  Created by Nicolas on 03/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef STRONGCAUSALITYEXPERIMENT_OBSERVER_H
#define STRONGCAUSALITYEXPERIMENT_OBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "WorldModels/EvolvingRobotAgentWorldModel.h"


class StrongCausalityExperimentAgentObserver : public AgentObserver
{		
	protected:
		EvolvingRobotAgentWorldModel	*_wm;
		int								_lifetimeIterations;

		std::vector<double>				_storedGenome;

		int								_testIt;			// current genome iteration (count nb of genome evaluated)
		double							_sumOfFitnesses;	// sum of all fitnesses for current genome

		double							_sigma;
		double							_sigmaMin;
		double							_sigmaMax;
		
		double							_maxValue;
		double							_minValue;
		
		int								_triesPerSigma;
		int								_nbOfTriesPerSigmaValue;

		
		void mutateWithBouncingBounds();

	public:
		StrongCausalityExperimentAgentObserver( RobotAgentWorldModel *__wm );
		~StrongCausalityExperimentAgentObserver();
				
		void reset();
		void step();
		
		void setRandomGenome();
		void updateFitness();


};


#endif

