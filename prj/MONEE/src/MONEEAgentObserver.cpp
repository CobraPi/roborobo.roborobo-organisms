#include "MONEE/include/MONEEAgentObserver.h"

MONEEAgentObserver::MONEEAgentObserver( )
{
}

MONEEAgentObserver::MONEEAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (MONEEAgentWorldModel*)__wm;
}

MONEEAgentObserver::~MONEEAgentObserver()
{
	// nothing to do.
}

void MONEEAgentObserver::reset()
{
	// nothing to do.
}

void MONEEAgentObserver::step()
{
	// nothing to do.
}

