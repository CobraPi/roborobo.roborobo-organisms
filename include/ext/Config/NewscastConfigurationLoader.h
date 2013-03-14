/*
 *  NewscastConfigurationLoader.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEWSCASTCONFIGURATIONLOADER_H
#define	NEWSCASTCONFIGURATIONLOADER_H

#include "Config/PropertiesReadingConfigurationLoader.h"
#include "Utilities/ExtendedProperties.h"
#include "Utilities/Misc.h" // contains convertFromString

class NewscastConfigurationLoader : public PropertiesReadingConfigurationLoader
{
public:
	NewscastConfigurationLoader();
	virtual ~NewscastConfigurationLoader();
	
	WorldObserver *make_WorldObserver(World* wm) ;
	RobotAgentWorldModel *make_RobotAgentWorldModel();
	AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
	BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
	RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
	
	bool readProperties(ExtendedProperties *gProperties);
	
};

#endif
