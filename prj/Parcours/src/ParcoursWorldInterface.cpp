/*
 *  ParcoursWorldInterface.cpp
 *  roborobo
 *
 *  Created by B.P.M. Weel on 3/20/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "Parcours/include/ParcoursWorldInterface.h"
#include "World/World.h"
#include "Parcours/include/ParcoursSharedData.h"
#include "Parcours/include/ParcoursAgentWorldModel.h"

void ParcoursWorldInterface::prepareInternalDynamics (){
}

void ParcoursWorldInterface::applyInternalDynamics( RobotAgentWorldModel* wm){
	/** Ditch Implementation.
	resetAgents();
	
	for (int i = 0; i < gWorld->getNbOfAgent(); i++) {
		RobotAgentPtr agent = gWorld->getAgent(i);
		ParcoursAgentWorldModel *worldModel = dynamic_cast<ParcoursAgentWorldModel*> (agent->getWorldModel());
		
		if (agent->isPartOfOrganism() && !worldModel->ditchCheck) {
			checkDitchOrganism(agent->getOrganism());
		}else if(!worldModel->ditchCheck && checkDitch(worldModel->_xReal, worldModel->_yReal)) {
			// Robot is in the ditch and not in organism. Give it a penalty and re-place it.
			worldModel->penalty = 1000;
			rePlaceRobot(agent);
			worldModel->ditchCheck = true;
		}
	}
	*/
	
	RobotAgentWorldInterface::applyInternalDynamics(wm);
}

void ParcoursWorldInterface::rePlaceRobot(RobotAgentPtr agent) {
	agent->unregisterAgent();
	do {
		int x = (int) 20 + (ranf() * (double) (gSpawnWidth));
		int y = (int) 20 + (ranf() * (double) (gSpawnHeight));
		agent->setCoordReal(x, y);
		agent->setCoord(x, y);
        
		Point2d pos(x,y);
		agent->oldPosition = pos;
	}while (agent->isCollision());
	agent->registerAgent();
}

bool ParcoursWorldInterface::checkDitch(double x, double y){
	Uint8 r, g, b;
	Uint32 pixel = getPixel32(gZoneImage, x, y);
	SDL_GetRGB(pixel, gZoneImage->format, &r, &g, &b);
	return (r == ParcoursSharedData::ZONE_VALUE);
}

void ParcoursWorldInterface::resetAgents() {
	for (int i = 0; i < gWorld->getNbOfAgent(); i++) {
		RobotAgentPtr agent = gWorld->getAgent(i);
		//ParcoursAgentWorldModel *worldModel = dynamic_cast<ParcoursAgentWorldModel*> (agent->getWorldModel());
		// Reset ditch check
		//worldModel->ditchCheck = false;
	}
}

void ParcoursWorldInterface::checkDitchOrganism(Organism *organism){
	double x,y;
	organism->getBaryCenter(x,y);

	if(checkDitch(x, y)){
		// The bary center is over the ditch, check for support on both sides
		bool leftSupport = false;
		bool rightSupport = false;
		for (size_t i=0; i<organism->agents.size(); i++) {
			/*ParcoursAgentWorldModel *wm = dynamic_cast<ParcoursAgentWorldModel*> (organism->agents[i]->getWorldModel());
			if(!checkDitch(wm->_xReal - (gAgentWidth/2), wm->_yReal)){
				// the left this robot is not in the ditch, so supporting on side or the other
				leftSupport = true;
			}else if (!checkDitch(wm->_xReal + (gAgentWidth/2), wm->_yReal)){
				// right of the gap
				rightSupport = true;
			}
			wm->ditchCheck = true;*/
		}
		if(!leftSupport || !rightSupport){
			while(organism->agents.size()>0) {
				// one of the sides is not support. Organism is falling in. Relocate each robot
				RobotAgentPtr robot = organism->agents.front();
				//dynamic_cast<ParcoursAgentWorldModel*> (robot->getWorldModel())->penalty += 3000;
                //robot->letGoOfOrganism();
				//rePlaceRobot(robot);
			}
		}
	}else{			
		for (size_t i=0; i<organism->agents.size(); i++) {
			// we have checked all the agents in the organism
			//dynamic_cast<ParcoursAgentWorldModel*> (organism->agents[i]->getWorldModel())->ditchCheck = true;
		}
	}
}

