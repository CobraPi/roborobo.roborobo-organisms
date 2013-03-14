/*
 *  MedeaTwoSunsPerceptronControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MedeaTwoSunsENERGYPERCEPTRONCONTROLARCHITECTURE_H
#define MedeaTwoSunsENERGYPERCEPTRONCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsAgentWorldModel.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsAgentObserver.h"

class MedeaTwoSunsPerceptronControlArchitecture : public BehaviorControlArchitecture
{
	private:

		int _iteration;
		MedeaTwoSunsAgentWorldModel *_wm;

		std::vector<double> _parameters;
		int _nbHiddenNeurons;

		void searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints);

	public:
		//Initializes the variables
		MedeaTwoSunsPerceptronControlArchitecture( RobotAgentWorldModel *__wm );
		~MedeaTwoSunsPerceptronControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

};


#endif

