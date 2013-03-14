#ifndef ParcoursWORLDOBSERVER_H
#define ParcoursWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

class ParcoursWorldObserver : public WorldObserver  {
public:
    ParcoursWorldObserver(World *__world);
    virtual ~ParcoursWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
};

#endif

