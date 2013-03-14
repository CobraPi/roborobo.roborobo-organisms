
#ifndef RIVERBEDAGENTOBSERVER_H
#define RIVERBEDAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Riverbed/include/RiverbedAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class RiverbedAgentObserver : public AgentObserver {
protected:
    RiverbedAgentWorldModel *_wm;

public:
    //Initializes the variables
    RiverbedAgentObserver();
    RiverbedAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~RiverbedAgentObserver();

    void reset();
    void step();
};

#endif

