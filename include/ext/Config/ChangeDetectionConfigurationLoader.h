/*
 * ChangeDetectionConfigurationLoader.h
 *
 */

#ifndef ChangeDetectionCONFIGURATIONLOADER_H
#define	ChangeDetectionCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "PropertiesReadingConfigurationLoader.h"

class ChangeDetectionConfigurationLoader : public PropertiesReadingConfigurationLoader {
public:
	ChangeDetectionConfigurationLoader();
    ~ChangeDetectionConfigurationLoader();

    WorldObserver *make_WorldObserver(World* wm);
    RobotAgentWorldModel *make_RobotAgentWorldModel();
    AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm);
    BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm);
    RobotAgentWorldInterface *make_RobotAgentWorldInterface(); // interface created once in constructor, returns a pointer

    bool readProperties(ExtendedProperties *gProperties);
};

#endif
