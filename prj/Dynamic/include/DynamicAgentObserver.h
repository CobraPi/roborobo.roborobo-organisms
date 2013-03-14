
#ifndef DYNAMICAGENTOBSERVER_H
#define DYNAMICAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Dynamic/include/DynamicAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class DynamicAgentObserver : public AgentObserver {
protected:
    DynamicAgentWorldModel *_wm;

public:
    //Initializes the variables
    DynamicAgentObserver();
    DynamicAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~DynamicAgentObserver();

    void reset();
    void step();
    
    void redistributeEnergy(Organism *organism);
};

#endif

