#ifndef ChangeDetectionWORLDOBSERVER_H
#define ChangeDetectionWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

class ChangeDetectionWorldObserver : public WorldObserver  {
public:
    ChangeDetectionWorldObserver(World *__world);
    virtual ~ChangeDetectionWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
};

#endif

