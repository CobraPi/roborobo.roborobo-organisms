
#ifndef ParcoursAGENTOBSERVER_H
#define ParcoursAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Parcours/include/ParcoursAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class ParcoursAgentObserver : public AgentObserver {
protected:
    ParcoursAgentWorldModel *_wm;

public:
    //Initializes the variables
    ParcoursAgentObserver();
    ParcoursAgentObserver(RobotAgentWorldModel *__wm);
    virtual ~ParcoursAgentObserver();

    void reset();
    void step();
};

#endif

