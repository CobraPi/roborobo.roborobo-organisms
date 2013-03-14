#include "World/World.h"

#include "Symbrion/include/SymbrionAgentObserver.h"
#include "Symbrion/include/SymbrionSharedData.h"

SymbrionAgentObserver::SymbrionAgentObserver()
{
}

SymbrionAgentObserver::SymbrionAgentObserver( RobotAgentWorldModel* __wm )
{
	this->_wm = (SymbrionAgentWorldModel*)__wm;
}

SymbrionAgentObserver::~SymbrionAgentObserver()
{
	// nothing to do.
}

void SymbrionAgentObserver::reset()
{
	// nothing to do.
}

void SymbrionAgentObserver::step()
{
	// nothing to do.
}