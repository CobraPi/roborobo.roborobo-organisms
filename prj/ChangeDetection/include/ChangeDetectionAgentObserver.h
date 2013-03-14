
#ifndef ChangeDetectionAGENTOBSERVER_H
#define ChangeDetectionAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "ChangeDetection/include/ChangeDetectionAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class ChangeDetectionAgentObserver : public AgentObserver {
protected:
    ChangeDetectionAgentWorldModel *_wm;

public:
    //Initializes the variables
    ChangeDetectionAgentObserver();
    ChangeDetectionAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~ChangeDetectionAgentObserver();

    void reset();
    void step();
};

#endif

