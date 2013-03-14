/*
 *  HiveMindConfigurationLoader.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HIVEMINDCONFIGURATIONLOADER_H
#define	HIVEMINDCONFIGURATIONLOADER_H

#include "Config/PropertiesReadingConfigurationLoader.h"
#include "Utilities/ExtendedProperties.h"
#include "Utilities/Misc.h" // contains convertFromString

class HiveMindConfigurationLoader : public PropertiesReadingConfigurationLoader
{
public:
	HiveMindConfigurationLoader();
	~HiveMindConfigurationLoader();
	
	WorldObserver *make_WorldObserver(World* wm) ;
	RobotAgentWorldModel *make_RobotAgentWorldModel();
	AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
	BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
	RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
	
	bool readProperties(ExtendedProperties *gProperties);
	
};

#endif
