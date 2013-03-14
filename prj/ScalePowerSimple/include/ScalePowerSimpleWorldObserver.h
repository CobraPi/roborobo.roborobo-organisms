#ifndef ScalePowerSimpleWORLDOBSERVER_H
#define ScalePowerSimpleWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

class ScalePowerSimpleWorldObserver : public WorldObserver {
protected:

public:
    ScalePowerSimpleWorldObserver(World *__world);
    ~ScalePowerSimpleWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
    void updateAllAgentsEnergy();
    double getScalePowerSimpleSupply(Organism *organism, int powerSupply);
};

#endif

