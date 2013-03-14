#ifndef SCALEPOWERWORLDOBSERVER_H
#define SCALEPOWERWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

class ScalePowerWorldObserver : public WorldObserver {
protected:

public:
    ScalePowerWorldObserver(World *__world);
    virtual ~ScalePowerWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
    void updateAllAgentsEnergy();
    double getScalePowerSupply(Organism *organism, int powerSupply);
};

#endif

