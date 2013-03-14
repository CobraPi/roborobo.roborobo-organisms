
#ifndef ChangingWorldAGENTOBSERVER_H
#define ChangingWorldAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "ChangingWorld/include/ChangingWorldAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class ChangingWorldAgentObserver : public AgentObserver {
protected:
    ChangingWorldAgentWorldModel *_wm;

public:
    //Initializes the variables
    ChangingWorldAgentObserver();
    ChangingWorldAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~ChangingWorldAgentObserver();

    void reset();
    void step();
};

#endif

