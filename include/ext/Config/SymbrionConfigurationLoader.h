/*
 * SymbrionConfigurationLoader.h
 *
 */

#ifndef SYMBRIONCONFIGURATIONLOADER_H
#define	SYMBRIONCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class SymbrionConfigurationLoader : public ConfigurationLoader {
public:
    SymbrionConfigurationLoader();
    ~SymbrionConfigurationLoader();

    WorldObserver *make_WorldObserver(World* wm);
    RobotAgentWorldModel *make_RobotAgentWorldModel();
    AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm);
    BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm);
    RobotAgentWorldInterface *make_RobotAgentWorldInterface(); // interface created once in constructor, returns a pointer

    bool readProperties();
};

#endif