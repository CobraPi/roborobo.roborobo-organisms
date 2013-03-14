/*
 * RiverbedConfigurationLoader.h
 *
 */

#ifndef RIVERBEDCONFIGURATIONLOADER_H
#define	RIVERBEDCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class RiverbedConfigurationLoader : public ConfigurationLoader {
public:
    RiverbedConfigurationLoader();
    ~RiverbedConfigurationLoader();

    WorldObserver *make_WorldObserver(World* wm);
    RobotAgentWorldModel *make_RobotAgentWorldModel();
    AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm);
    BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm);
    RobotAgentWorldInterface *make_RobotAgentWorldInterface(); // interface created once in constructor, returns a pointer

    bool readProperties();
};

#endif