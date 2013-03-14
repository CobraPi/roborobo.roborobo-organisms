
#ifndef ScalePowerSimpleAGENTOBSERVER_H
#define ScalePowerSimpleAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class ScalePowerSimpleAgentObserver : public AgentObserver {
protected:
    ScalePowerSimpleAgentWorldModel *_wm;

public:
    //Initializes the variables
    ScalePowerSimpleAgentObserver();
    ScalePowerSimpleAgentObserver(RobotAgentWorldModel *__wm);
    ~ScalePowerSimpleAgentObserver();

    void reset();
    void step();
};

#endif

