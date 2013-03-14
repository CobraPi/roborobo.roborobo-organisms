#ifndef RIVERBEDWORLDOBSERVER_H
#define RIVERBEDWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

class RiverbedWorldObserver : public WorldObserver {
protected:

public:
    RiverbedWorldObserver(World *__world);
    virtual ~RiverbedWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
    void updateAllAgentsEnergy();
    double getRiverbedSupply(Organism *organism, RobotAgentPtr agent);
};

#endif

