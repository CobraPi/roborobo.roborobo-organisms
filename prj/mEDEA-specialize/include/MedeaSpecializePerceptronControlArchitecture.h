/*
 *  MedeaSpecializePerceptronControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 15/09/10
 *
 */


#ifndef MEDEASPECIALIZE_PERCEPTRONCONTROLARCHITECTURE_H
#define MEDEASPECIALIZE_PERCEPTRONCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "mEDEA-specialize/include/MedeaSpecializeAgentWorldModel.h"
#include "mEDEA-specialize/include/MedeaSpecializeAgentObserver.h"

class MedeaSpecializePerceptronControlArchitecture : public BehaviorControlArchitecture
{
	private:

		int _iteration;
		MedeaSpecializeAgentWorldModel *_wm;

		std::vector<double> _parameters;
		int _nbHiddenNeurons;

		void searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints);

	public:
		//Initializes the variables
		MedeaSpecializePerceptronControlArchitecture( RobotAgentWorldModel *__wm );
		~MedeaSpecializePerceptronControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

};


#endif

