/*
 *  SelfRepPerceptronControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef SELFREPPERCEPTRONCONTROLARCHITECTURE_H
#define SELFREPPERCEPTRONCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "SelfReplication/include/SelfRepAgentWorldModel.h"
#include "WorldModels/RobotAgentWorldModel.h"
#include "SelfReplication/include/SelfRepAgentObserver.h"



class SelfRepPerceptronControlArchitecture : public BehaviorControlArchitecture
{
	private:
		//TODO : RobotAgentWorldModel* faire des cast pour voir si c'est bon
		RobotAgentWorldModel *_wm;
		SelfRepAgentWorldModel *_selfRepWm;
		std::vector<double> _currentGenome;

		int _iterationMax;

		double _maxKeyRange;

		double _minValue;
		double _maxValue;
		double _nbGene;

		double _angleToClosestEnergyPoint;
		double _distanceToClosestEnergyPoint;

		void searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints);

		void stepPerceptron();
		void resetActiveGenome();

	public:
		//Initializes the variables
		SelfRepPerceptronControlArchitecture( RobotAgentWorldModel *__wm );
		~SelfRepPerceptronControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

		std::vector<double> mutateWithBouncingBounds( float sigma);
};


#endif

