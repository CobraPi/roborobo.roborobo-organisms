
#include "World/World.h"
#include "SDL_collide.h"


#include "ScalePowerSimple/include/ScalePowerSimpleAgentObserver.h"



ScalePowerSimpleAgentObserver::ScalePowerSimpleAgentObserver( )
{
}

ScalePowerSimpleAgentObserver::ScalePowerSimpleAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (ScalePowerSimpleAgentWorldModel*)__wm;
}

ScalePowerSimpleAgentObserver::~ScalePowerSimpleAgentObserver()
{
	// nothing to do.
}

void ScalePowerSimpleAgentObserver::reset()
{
	// nothing to do.
}

void ScalePowerSimpleAgentObserver::step()
{
	// nothing to do.
}