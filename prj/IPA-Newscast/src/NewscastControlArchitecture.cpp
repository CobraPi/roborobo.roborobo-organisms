/*
 *  NewscastControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Robert-Jan on 07/10/2010
 *
 */

#include "IPA-Newscast/include/NewscastControlArchitecture.h"

int NewscastControlArchitecture::cacheSize = 4; // This should be changed by the properties file
int NewscastControlArchitecture::itemTTL = 2; // This should be changed by the properties file

NewscastControlArchitecture::NewscastControlArchitecture( RobotAgentWorldModel *__wm ) : PopulationControlArchitecture ( __wm )
{
	setPopulation(new NewscastAgentGroup(cacheSize, itemTTL, cacheSize));
}

NewscastControlArchitecture::~NewscastControlArchitecture()
{
	// nothing here
}

ControllerPtr NewscastControlArchitecture::createRandomGenome(){
	return Util::createRandomGenome(_wm);
}
