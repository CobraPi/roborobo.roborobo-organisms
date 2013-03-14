/*
 *  MedeaPerceptronControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MedeaENERGYPERCEPTRONCONTROLARCHITECTURE_H
#define MedeaENERGYPERCEPTRONCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"
#include "mEDEA/include/MedeaAgentObserver.h"

class MedeaPerceptronControlArchitecture : public BehaviorControlArchitecture
{
	private:

		int _iteration;
		MedeaAgentWorldModel *_wm;

		std::vector<double> _parameters;
		int _nbHiddenNeurons;

		void searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints);

	public:
		//Initializes the variables
		MedeaPerceptronControlArchitecture( RobotAgentWorldModel *__wm );
		~MedeaPerceptronControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

};


#endif

