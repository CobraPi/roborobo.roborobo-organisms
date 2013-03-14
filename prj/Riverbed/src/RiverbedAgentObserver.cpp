
#include "World/World.h"
#include "SDL_collide.h"


#include "Riverbed/include/RiverbedAgentObserver.h"
#include "Riverbed/include/RiverbedSharedData.h"


RiverbedAgentObserver::RiverbedAgentObserver( )
{
}

RiverbedAgentObserver::RiverbedAgentObserver( RobotAgentWorldModel *__wm )
{
	this->_wm = (RiverbedAgentWorldModel*)__wm;
}

RiverbedAgentObserver::~RiverbedAgentObserver()
{
	// nothing to do.
}

void RiverbedAgentObserver::reset()
{
	// nothing to do.
}

void RiverbedAgentObserver::step()
{
	// nothing to do.  
}