#ifndef DYNAMICWORLDOBSERVER_H
#define DYNAMICWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Organism.h"

class World;

#define MAX_COLLISIONS          1500

#define OBSTACLE_MIN_SPEED		0.1
#define OBSTACLE_MAX_SPEED		1.0

class DynamicWorldObserver : public WorldObserver {
private:

    int yintercept;
    
    void initializePlants();
    void updatePlants();
public:
    DynamicWorldObserver(World *__world);
    virtual ~DynamicWorldObserver();
    
    void showOrganisms();

    void reset();
    void step();
    void updateAllAgentsEnergy();
};

#endif

