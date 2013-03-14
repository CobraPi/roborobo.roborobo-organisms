/*
 * DynamicConfigurationLoader.h
 *
 */

#ifndef DYNAMICCONFIGURATIONLOADER_H
#define	DYNAMICCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "PropertiesReadingConfigurationLoader.h"

class DynamicConfigurationLoader : public PropertiesReadingConfigurationLoader {
public:
    DynamicConfigurationLoader();
    ~DynamicConfigurationLoader();

    WorldObserver *make_WorldObserver(World* wm);
    RobotAgentWorldModel *make_RobotAgentWorldModel();
    AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm);
    BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm);
    RobotAgentWorldInterface *make_RobotAgentWorldInterface(); // interface created once in constructor, returns a pointer

    bool readProperties(ExtendedProperties *gProperties);
};

#endif