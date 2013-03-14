/*
 *  NewscastAgentWorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */


#ifndef NEWSCASTAGENTWORLDMODEL_H
#define NEWSCASTAGENTWORLDMODEL_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

class World;

class NewscastAgentWorldModel : public RobotAgentWorldModel
{	
	private: 
		int _robotNeighborhoodCounter; // count the number of other robots within a given radius.
	
	public:

		// refer to super class to check what data / methods are available.		
		
		NewscastAgentWorldModel();
		virtual ~NewscastAgentWorldModel();
};


#endif


