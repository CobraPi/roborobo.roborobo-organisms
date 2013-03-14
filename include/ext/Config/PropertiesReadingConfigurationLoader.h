/*
 *  PropertiesReadingConfigurationLoader.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 25-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROPERTIESREADINGCONFIGURATIONLOADER_H
#define	PROPERTIESREADINGCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "Utilities/ExtendedProperties.h"

class PropertiesReadingConfigurationLoader : public ConfigurationLoader
{
public:
	PropertiesReadingConfigurationLoader();
	virtual ~PropertiesReadingConfigurationLoader();
	
	virtual WorldObserver* make_WorldObserver(World* wm) = 0 ;
	virtual RobotAgentWorldModel* make_RobotAgentWorldModel() = 0 ;
	virtual AgentObserver* make_AgentObserver(RobotAgentWorldModel* wm) = 0 ;
	virtual BehaviorControlArchitecture* make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) = 0;
	virtual RobotAgentWorldInterface* make_RobotAgentWorldInterface() = 0; // interface created once in constructor, returns a pointer
	
	virtual bool readProperties(ExtendedProperties *gProperties) = 0;
};

#endif

