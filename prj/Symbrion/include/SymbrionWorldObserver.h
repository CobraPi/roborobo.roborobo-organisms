#ifndef SYMBRIONWORLDOBSERVER_H
#define SYMBRIONWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

#include "Symbrion/include/Agents/SymbrionRobotAgent.h"

class World;

class SymbrionWorldObserver : public WorldObserver {
public:
	SymbrionWorldObserver( World* __world );
	virtual ~SymbrionWorldObserver();
	
	void showOrganisms();
	
	void reset();
	void step();
    
private:
    void logHeatmap();
    void resetHeatmap();
    
    bool loadFiles();
    void updateWorld();
    void initWorld();
    
    void boardcastSeed(int index);
    void boardcastOrganism(int index);
    
    SymbrionRobotAgentPtr getAgent(int index);
    
    std::vector< std::vector<int> > _heatmap;
};

#endif
