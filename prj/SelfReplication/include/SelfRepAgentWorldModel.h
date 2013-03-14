/*
 *  SelfRepAgentWorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 22/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef SELFREPAGENTWORLDMODEL_H
#define SELFREPAGENTWORLDMODEL_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/EvolvingRobotAgentWorldModel.h"

//class World;

class SelfRepAgentWorldModel : public EvolvingRobotAgentWorldModel
{
	protected:
		double _energy;
		double _deltaEnergy;

		double _deltaKey;
		double _key;

		double _sigmaRef;

		bool _stillAlive;
		bool _duplicate;

		int _uniqueAgentId;
		static int _nextId;
		
	public:
		//Initializes the variables
		SelfRepAgentWorldModel();
		~SelfRepAgentWorldModel();

		double getEnergy();
		void setEnergy(double energy);

		double getDeltaEnergy();
		void setDeltaEnergy(double deltaEnergy);

		double getKey();
		void setKey(double key);
	
		double getDeltaKey();
		void setDeltaKey(double deltaKey);

		bool getStillAlive();
		void setStillAlive(bool stillAlive);
	
		bool getDuplicate();
		void setDuplicate(bool duplicate);
	
		int getUniqueAgentId();

		int getAge();
		void setAge(int age);
};


#endif

