#ifndef SYMBRIONAGENTOBSERVER_H
#define SYMBRIONAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Symbrion/include/SymbrionAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class SymbrionAgentObserver : public AgentObserver {
protected:
	SymbrionAgentWorldModel* _wm;
	
public:
	SymbrionAgentObserver();
	SymbrionAgentObserver( RobotAgentWorldModel* __wm );
	virtual ~SymbrionAgentObserver();
	
	void reset();
	void step();
};

#endif