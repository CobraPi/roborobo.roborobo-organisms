#ifndef SYMBRIONROBOTAGENT_H
#define SYMBRIONROBOTAGENT_H

#include "Agents/RobotAgent.h"
#include "Symbrion/include/SymbrionAgentWorldModel.h"
#include "Symbrion/include/SymbrionOrganismGenome.h"
#include "Symbrion/include/SymbrionSharedData.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

class SymbrionRobotAgent : public RobotAgent, public boost::enable_shared_from_this<SymbrionRobotAgent>
{
public:
	SymbrionRobotAgent(AgentState state, World *__world);
	
	virtual ~SymbrionRobotAgent();
	
	void setState( AgentState _state );
	AgentState getState() const;
	
	virtual void toSeed();
	virtual void toEgg();
    virtual void abortion();
	
	virtual bool isComplete();
	virtual void stepBehavior();
	virtual bool receiveGenome( SymbrionOrganismGenome* _genome );
	
	virtual int plannedSize();
    
    virtual SymbrionAgentWorldModel* getWorldModel();
    virtual SymbrionOrganismGenome* getGenome();
protected:
	AgentState _state;
	std::vector<SymbrionOrganismGenome*> _receivedGenomes;
	SymbrionOrganismGenome* _genome;
    
private:
    void setMask();
};

typedef boost::shared_ptr<SymbrionRobotAgent> SymbrionRobotAgentPtr;

#endif
