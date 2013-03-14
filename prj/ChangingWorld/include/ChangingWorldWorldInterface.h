/*
 *  ChangingWorldWorldInterface.h
 *  roborobo
 *
 *  Created by B.P.M. Weel on 3/20/12.
 *  Copyright 2012 VU. All rights reserved.
 *
 */
#ifndef ChangingWorldWORLDINTERFACEDANGERWORLD_H
#define ChangingWorldWORLDINTERFACEDANGERWORLD_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Organism.h"

#include "ChangingWorldAgentWorldModel.h"
#include "World/RobotAgentWorldInterface.h"

/* An AgentWorldInterface provides a single static class for applying world constrained dynamics onto agent dynamics
 Example: 
 add noise to effector values to compute "true" effector values
 add noise to sensor values
 modify "true" effector to comply with sliding, rugged terrain, etc.
 */
class ChangingWorldWorldInterface : public RobotAgentWorldInterface
{
public: 
	virtual void prepareInternalDynamics ();
	virtual void applyInternalDynamics( RobotAgentWorldModel* );
};

#endif

