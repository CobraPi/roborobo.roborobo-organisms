/*
 *  World.h
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef WORLD_H
#define WORLD_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Agents/RobotAgent.h"

#include "Utilities/Misc.h"

#include "Observers/WorldObserver.h"
#include "World/ResourceUpdater.h"
#include "World/ResourceFactory.h"

class RobotAgent;

class World
{
protected:
    
    std::vector<RobotAgentPtr> agents;
    
    int _iterations;
    
    //True if there is a variation in the number of agent
    bool _agentsVariation;
    bool _initializeAgents;
    bool _setUpConnections;
    bool _initializeEnergyPoints;
    
    std::vector<ResourceUpdaterPtr> resourceUpdaters;
    
    WorldObserver *_worldObserver;
    
public:
    
    World();
    ~World();
    
    bool loadFiles();
    //bool loadProperties( std::string __propertiesFilename );
    
    void initWorld();
    void resetWorld();
    void updateWorld(Uint8 *__keyboardStates = NULL);
    
    RobotAgentPtr getAgent( int __agentIndex );
    //bool isAgentRegistered( int __agentIndex );
    
    //delete an agent from the simulator. No other functions to call
    void deleteAgent (int __agentIndex );
    //add an agent in the simulator. No other functions to call
    void addAgent(RobotAgentPtr agentToAdd);
    
    int getIterations();
    WorldObserver* getWorldObserver();
    int getNbOfAgent();
    
	///added method -- Job
	std::vector<RobotAgentPtr>* listAgents();
		
    void setInitializeAgents(bool initAgents);
    bool getInitiliazeAgents();
    
    void setSetUpConnections(bool setUpConnections);
    bool getSetUpConnections();
    
    void setInitializeEnergyPoints(bool initEnergyPoints);
    bool getInitializeEnergyPoints();
    
    void registerResourceUpdater(ResourceUpdaterPtr);
    void unregisterResourceUpdater(ResourceUpdaterPtr);
};



#endif // WORLD_H

