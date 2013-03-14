/*
 *  NewscastControlArchitecture.h
 *  roborobo-online
 *
 */


#ifndef NEWSCASTCONTROLARCHITECTURE_H
#define NEWSCASTCONTROLARCHITECTURE_H 

#include <iostream>

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/NewscastAgent.h"
#include "newscast/NewscastAgentGroup.h"

using namespace std;

class NewscastControlArchitecture : public PopulationControlArchitecture
{
public:
	NewscastControlArchitecture( RobotAgentWorldModel *__wm );
	~NewscastControlArchitecture();
	
	virtual ControllerPtr createRandomGenome();
	
	// the following variables are set by the properties file
	static int cacheSize;
	static int itemTTL;
};


#endif

