/*
 *  BasicControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicProject/include/BasicControlArchitecture.h"
#include "Behavior/WallAvoidanceBehavior.h"

BasicControlArchitecture::BasicControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
	// nothing to do
}

BasicControlArchitecture::~BasicControlArchitecture()
{
	// nothing to do.
}

void BasicControlArchitecture::reset()
{
	// nothing to do.
}

void BasicControlArchitecture::step()
{
    WallAvoidanceBehavior::step(_wm);
}

