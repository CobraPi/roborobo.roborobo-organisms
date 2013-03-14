#ifndef ChangingWorldWORLDOBSERVER_H
#define ChangingWorldWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"
#include "World/World.h"

class ChangingWorldWorldObserver : public WorldObserver  {
public:
    ChangingWorldWorldObserver(World *__world);
    virtual ~ChangingWorldWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
    
private:
    void updateAllAgentsEnergy();
    double getChangingWorldSupply(Organism* organism, int powerSupply);
};

#endif

