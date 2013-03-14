
#ifndef SCALEPOWERAGENTOBSERVER_H
#define SCALEPOWERAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "ScalePower/include/ScalePowerAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class ScalePowerAgentObserver : public AgentObserver {
protected:
    ScalePowerAgentWorldModel *_wm;

public:
    //Initializes the variables
    ScalePowerAgentObserver();
    ScalePowerAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~ScalePowerAgentObserver();

    void reset();
    void step();
};

#endif

