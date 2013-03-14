/*
 *  OnePlusOneOnlineGeneticsEngineAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ONEPLUSONEONLINE_GENETICSENGINE_H
#define ONEPLUSONEONLINE_GENETICSENGINE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "WorldModels/EvolvingRobotAgentWorldModel.h"

class OnePlusOneOnlineGeneticsEngineAgentObserver : public AgentObserver
{		
	private: 
		void	resetFitness();
		void	updateFitness();
		double	getFitnessValue();
		void	resetActiveGenome();

		void mutateWithBouncingBounds();

	protected:
		EvolvingRobotAgentWorldModel	*_wm;
		std::vector<double>				_storedGenome;
		double							_storedGenomeFitnessValue;
		double							_storedGenomeCumulatedFitnessValue;

		double							_bestEverFitnessValue;

		bool							_reevalStoredGenome;
		int 							_evaluationCount;
		double							_reevaluationProbability;
		
		double							_sigma;
		double							_sigmaMin;
		double							_sigmaMax;
		
		double							_maxValue;
		double							_minValue;
		
		int								_randomMoveIterations;
		int								_recoveryIterations;
		int								_lifetimeIterations;


	public:
		OnePlusOneOnlineGeneticsEngineAgentObserver( RobotAgentWorldModel *__wm );
		~OnePlusOneOnlineGeneticsEngineAgentObserver();
				
		void reset();
		void step();

};


#endif

