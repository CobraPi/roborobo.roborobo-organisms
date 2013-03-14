
#include "World/World.h"
#include "SDL_collide.h"


#include "ScalePower/include/ScalePowerAgentObserver.h"



ScalePowerAgentObserver::ScalePowerAgentObserver( )
{
}

ScalePowerAgentObserver::ScalePowerAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (ScalePowerAgentWorldModel*)__wm;
}

ScalePowerAgentObserver::~ScalePowerAgentObserver()
{
	// nothing to do.
}

void ScalePowerAgentObserver::reset()
{
	// nothing to do.
}

void ScalePowerAgentObserver::step()
{
	// nothing to do.
}