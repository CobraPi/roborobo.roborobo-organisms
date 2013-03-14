//
//  WallAvoidanceBehavior.h
//  roborobo
//
//  Created by Berend Weel on 3/14/13.
//
//

#ifndef roborobo_WallAvoidanceBehavior_h
#define roborobo_WallAvoidanceBehavior_h

#include "WorldModels/RobotAgentWorldModel.h"

class WallAvoidanceBehavior
{
    public:
        static void step(RobotAgentWorldModel *_wm);
        static void stepSpeed(RobotAgentWorldModel *_wm, double speed);
        static void stepSpeedDifferential(RobotAgentWorldModel *_wm, double &left, double &right, double speed);
};

#endif
