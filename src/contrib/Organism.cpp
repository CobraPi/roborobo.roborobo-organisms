/*
 *  Organism.cpp
 *  roborobo
 *
 *  Created by Berend Weel.
 *
 */

#include "Organism.h"
#include "Agents/Agent.h"
#include "World/World.h"
#include <algorithm>
#include <vector>
#include <stack>
#include <cstdlib> 

bool gUseOrganisms = false;

std::string gAgentPositiveMaskImageFilename = "data/agent-positive-mask.png";
std::string gAgentNegativeMaskImageFilename = "data/agent-negative-mask.png";
std::string gOrganismSizesLogFilename = "logs/organism_sizes.log";
SDL_Surface *gAgentPositiveMaskImage = NULL;
SDL_Surface *gAgentNegativeMaskImage = NULL;
int gConnectionGap;
int gOrganismSampleFrequency = 250;

bool gUseOrganismLeader = false;
bool gOrganismNormaliseSpeed = false;

/**
 *  Globals definitions
 * 
 * Globals can used by all Organism functions as well as in projects
 * 
 * removeOrganism, addOrganism and destroyOrganisms functions
 * maintain a global set of organisms in the simulation
 * 
 * Below that is the implementation of the Organism Class
 */
int Organism::nbOrganisms = 0;
std::vector<Organism*> *Organism::organisms = NULL;
std::ofstream Organism::sizeLog;

bool Organism::loadProperties(){
    gUseOrganisms = false;
    bool returnValue = true;
    std::string s;
    
	s = gProperties.getProperty("gUseOrganisms");
	if (s == "true" || s == "True" || s == "TRUE") {
		gUseOrganisms = true;
	} else {
		if (s == "false" || s == "False" || s == "FALSE") {
			gUseOrganisms = false;
		} else {
			std::cerr << "[CORRUPT] gUseOrganisms should be boolean (\"true\" or \"false\"). Using default (\"false\").\n";
            //returnValue = false;
		}
	}
	if(gUseOrganisms) {
		if (gProperties.hasProperty("gAgentPositiveMaskImageFilename")){
			gAgentPositiveMaskImageFilename = gProperties.getProperty("gAgentPositiveMaskImageFilename");
		}else if(gUseOrganisms){
			std::cerr << "[MISSING] gAgentPositiveMaskImageFilename string value is missing.\n";
			if (gUseOrganisms)
				returnValue = false;
		}
		
		if (gProperties.hasProperty("gAgentNegativeMaskImageFilename")){
			gAgentNegativeMaskImageFilename = gProperties.getProperty("gAgentNegativeMaskImageFilename");
		}else if(gUseOrganisms){
			std::cerr << "[MISSING] gAgentNegativeMaskImageFilename string value is missing.\n";
			if (gUseOrganisms)
				returnValue = false;
		}
        
		
		if (gProperties.hasProperty("gConnectionGap"))
			convertFromString<int>(gConnectionGap, gProperties.getProperty("gConnectionGap"), std::dec);
		else if(gUseOrganisms){
			std::cerr << "[MISSING] gConnectionGap int value is missing, using default (20).\n";
			gConnectionGap = 20;
			//returnValue = false;
		}
		
		if (gProperties.hasProperty("gOrganismSizesLogFilename"))
			gOrganismSizesLogFilename = gProperties.getProperty("gOrganismSizesLogFilename");
		else {
			gOrganismSizesLogFilename = "logs/organisms_sizes_" + gStartTime + ".txt";
			gProperties.setProperty("gOrganismSizesLogFilename", gOrganismSizesLogFilename);
            
			std::cout << "[WARNING] No default gOrganismSizesLogFilename string value. Log data will be written in \"" << gOrganismSizesLogFilename << "\"\n";
			//returnValue = false;
		}
        
		if (gProperties.hasProperty("gOrganismSampleFrequency"))
			convertFromString<int>(gOrganismSampleFrequency, gProperties.getProperty("gOrganismSampleFrequency"), std::dec);
		else if(gUseOrganisms){
			std::cerr << "[MISSING] gOrganismSampleFrequency int value is missing, using default (250).\n";
			gOrganismSampleFrequency = 250;
			//returnValue = false;
		}
        
        s = gProperties.getProperty("gUseOrganismLeader");
        if (s == "true" || s == "True" || s == "TRUE") {
            gUseOrganismLeader = true;
        } else {
            if (s == "false" || s == "False" || s == "FALSE") {
                gUseOrganismLeader = false;
            } else {
                std::cerr << "[CORRUPT] gUseOrganismLeader should be boolean (\"true\" or \"false\"). Using default (\"false\").\n";
                //returnValue = false;
            }
        }
        s = gProperties.getProperty("gOrganismNormaliseSpeed");
        if (s == "true" || s == "True" || s == "TRUE") {
            gOrganismNormaliseSpeed = true;
        } else {
            if (s == "false" || s == "False" || s == "FALSE") {
                gOrganismNormaliseSpeed = false;
            } else {
                std::cerr << "[CORRUPT] gOrganismNormaliseSpeed should be boolean (\"true\" or \"false\"). Using default (\"false\").\n";
                //returnValue = false;
            }
        }
	}
    
    return returnValue;
}


void Organism::remove(Organism *organism) {
	std::vector<Organism*>::iterator it;
	for (it = organisms->begin(); it != organisms->end(); it++) {
		if (*it == organism) {
			organisms->erase(it);
			return; // do not continue, the iterator is now invalid
		}
	}
}

void Organism::add(Organism *organism) {
	organisms->push_back(organism);
}

void Organism::reset() {
	if (organisms != NULL) {
		Organism::destroy();
	}
	organisms = new std::vector<Organism*>();

	sizeLog.open(gOrganismSizesLogFilename.c_str());
	if (!sizeLog) {
		std::cout << "[error] Cannot open log file " << gOrganismSizesLogFilename << "." << std::endl << std::endl;
	}

	sizeLog << "Iteration,Organism,Size,Age,x,y,Agents" << std::endl;
}

void Organism::destroy() {
	std::vector<Organism*>::iterator it;
	for (it = organisms->begin(); it != organisms->end(); it++) {
		delete (*it);
	}
	delete organisms;
	organisms = NULL;
}

void Organism::showAll() {
	std::vector<Organism*>::iterator it;
	for (it = organisms->begin(); it != organisms->end(); it++) {
		(*it)->show();
	}
}

void Organism::logAll() {
	if (sizeLog) {
		std::vector<Organism*>::iterator it;
		for (it = organisms->begin(); it != organisms->end(); it++) {
			(*it)->logOrganism();
		}
	}
}

/**
 *  Class Implementation
 */
Organism::Organism() {
	this->id = ++Organism::nbOrganisms;
	//this->agents = new std::vector<RobotAgentPtr>();
	this->hasMoved = false;
	this->creationIteration = gWorld->getIterations();
	this->active = true;
    this->age = 0;
}

Organism::~Organism() {
}

void Organism::setId(int _id) {
	this->id = _id;
}

int Organism::getId() {
	return this->id;
}

unsigned int Organism::getAge() {
	return this->age;
}

void Organism::update()
{
    if (gWorld->getIterations() % gOrganismSampleFrequency == 0){
        this->logOrganism();
    }
	this->age++;
}

void Organism::addRobot(RobotAgentPtr robot) {
    // Mark: Adding a robot to an organism now implies that in case the organsim consists of a single agent the leader should be determined. Otherwise, the leader has already been determined.
    
    if (this->agents.size() == 0){
        // If this is the first robot to join the organsism, this is the leader.
        this->leader = robot;
    }else{
        // Otherwise we will just stick to the previous leader.
    }
	
    this->agents.push_back(robot);
}

void Organism::removeRobot(RobotAgentPtr robot) {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = this->agents.begin(); it != this->agents.end(); it++) {
		if ((*it)->getWorldModel()->_agentId == robot->getWorldModel()->_agentId) {
			this->agents.erase(it);
			break;
		}
	}
	robot->setOrganism(NULL);
    
	// Remove the organism if there is less than 1 robot left in it
	if (this->agents.size() < 1) {
		this->leader = RobotAgentPtr();
		this->setInactive();
	}else{
        // Create another leader in case the leader has just been removed. We'll just take the first one in the agents vector.
        if (this->leader == robot){
            this->leader = agents.at(0);
        }
    }
}

bool Organism::contains(RobotAgentPtr robot) {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = this->agents.begin(); it != this->agents.end(); it++) {
		if ((*it)->getWorldModel()->_agentId == robot->getWorldModel()->_agentId) {
			return true;
		}
	}
	return false;
}

void Organism::prepareAllMove() {
	std::vector<Organism*>::iterator it;
	for (it = organisms->begin(); it != organisms->end(); it++) {
		(*it)->prepareMove();
        (*it)->update();
	}
}

void Organism::checkAllIntegrity() {
	std::vector<Organism*>::iterator it;
	std::vector<Organism*> neworgs = std::vector<Organism*>();
	for (it = organisms->begin(); it != organisms->end(); it++) {
		std::vector<Organism*> tmp = (*it)->checkIntegrity();
		neworgs.insert(neworgs.end(), tmp.begin(), tmp.end());
	}
	organisms->insert(organisms->end(), neworgs.begin(), neworgs.end());
}

int Organism::size() {
	return this->agents.size();
}

void Organism::setActive() {
	this->active = true;
}

void Organism::setInactive() {
	this->agents.clear();
	this->active = false;
	this->deathIteration = gWorld->getIterations();
}

bool Organism::isActive() {
	return this->active;
}

RobotAgentPtr Organism::getLeader(){
    return leader;
}

void Organism::setLeader(RobotAgentPtr __leader){
    leader = __leader;
}

/**
 * Merges the organism into another organism. Do not forget to
 * deallocate the organism that merged into the other.
 * 
 * @param other the Organism to merge into
 */
void Organism::mergeInto(Organism* other) {
    // We're going to merge two organisms. The leader should be the one with the highest number of agents.
  
    if (other->agents.size() > agents.size()){
        this->leader = other->leader;
    }
    
	other->agents.insert(other->agents.end(), this->agents.begin(), this->agents.end());
	std::vector<RobotAgentPtr>::iterator it;
	for (it = this->agents.begin(); it != this->agents.end(); it++) {
		(*it)->setOrganism(other);
        (*it)->getWorldModel()->joinedOrganism = true;
	}
	gLogFile << "Organism: " << this->id << " created at iteration: " << this->creationIteration << " destroyed at iteration " << gWorld->getIterations()<< std::endl;
}

/**
 * Check whether the organism is still connected
 */
std::vector<Organism*> Organism::checkIntegrity() {
	std::vector<Organism*> orgs = std::vector<Organism*>();
	if (!isActive()) {
		return orgs;
	}

	std::vector<RobotAgentPtr> not_visited = std::vector<RobotAgentPtr>(this->agents);
	std::vector<RobotAgentPtr> loners = std::vector<RobotAgentPtr>();

	std::vector<RobotAgentPtr> connected = std::vector<RobotAgentPtr>();
	std::stack<RobotAgentPtr> s = std::stack<RobotAgentPtr>();

	// Tree visiting algorithm using a stack
	// All nodes of the "tree" should be visited, otherwise
	// the organism is falling apart
	// Repeat until all robots in the (ex)organism are accounted for
	while (!not_visited.empty()) {
		s.push(not_visited.front());
		while (!s.empty()) {
			RobotAgentPtr a = s.top();
			s.pop();

			// This robot has been visited
			std::vector<RobotAgentPtr>::iterator it;
			it = std::find(not_visited.begin(), not_visited.end(), a);
			if (it != not_visited.end()) {
				not_visited.erase(it);
			}

			// so it is connected to the current organism
			connected.push_back(a);

			// Add the neighbours of this robot to the stack
			std::vector<RobotAgentPtr>::iterator it2;
			for (it2 = a->getConnected()->begin(); it2 != a->getConnected()->end(); it2++) {
				if (std::find(connected.begin(), connected.end(), (*it2)) == connected.end()) {
					s.push((*it2));
				}
			}
		}

		if (connected.size() == this->agents.size()) {
			// organism intact.
			return orgs;
		} else if (connected.size() > 1) {
			// a connected part of the organism exist
			Organism *o = new Organism();
			std::vector<RobotAgentPtr>::iterator it;
			for (it = connected.begin(); it != connected.end(); it++) {
				o->addRobot((*it));
				(*it)->setOrganism(o);
			}
			orgs.push_back(o);
		} else if (connected.size() == 1) {
			// this robot is not connected to any other
			connected.front()->letGoOfOrganism();
			loners.push_back(connected.front());
		}
		connected.clear();
	}

	std::vector<RobotAgentPtr>::iterator it2;
	for (it2 = loners.begin(); it2 != loners.end(); it2++) {
		(*it2)->setOrganism(NULL);
	}
	this->setInactive();

	return orgs;
}

/**
 *  Initialize values needed before movement
 */
void Organism::prepareMove() {
	if (this->isActive()) {
		this->hasMoved = false;
	}
}

/**
 * Perform the actual movement
 */
void Organism::move() {
	// Do not move more than once in an update
	if (hasMoved || !active)
		return;

    double angle, translation = 0;
	// Calculate the combined angle and speed of the robots
	combineVectors(angle, translation);

	// Move all the agents using the combined angle
	// and speed.
	moveOrganism(angle, translation);

	// Did we collide with anything?
	if (detectCollisions()) {
		// Then roll the movement back
		rollbackMove();
	} else {
		updateWorldModels();
	}

	// Anyway, we are done moving this movement
	this->hasMoved = true;

	// So update the sensors
	updateSensors();
}

void Organism::show() {
	if (isActive()) {
		std::cout << "Organism: " << this->getId() << " agents:";
		std::vector<RobotAgentPtr>::iterator it;
		for (it = this->agents.begin(); it != this->agents.end(); it++) {
			std::cout << " " << (*it)->getWorldModel()->_agentId;
		}
		std::cout << std::endl;
	}
	sizeLog << std::endl;
}

void Organism::logOrganism() {
	if (isActive()) {
		double x,y;
		this->getBaryCenter(x,y);
		sizeLog << gWorld->getIterations() << "," << this->getId();
		sizeLog << "," << agents.size();
		sizeLog << "," << (gWorld->getIterations() - this->creationIteration);
		sizeLog << "," << x << "," << y;

		std::vector<RobotAgentPtr>::iterator it;
		for (it = this->agents.begin(); it != this->agents.end(); it++) {
			sizeLog << "," << (*it)->getWorldModel()->_agentId;
		}
		sizeLog << std::endl;
	}
}

/**
 * Combines the vectors of the robots in the organism and
 * also unregisters each agent.
 * @param angle
 * @param translation
 */
void Organism::combineVectors(double &angle, double &translation) {
	std::vector<RobotAgentPtr>::iterator it;
	double Rx = 0, Ry = 0;

    for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel* worldModel = (*it)->getWorldModel();

		(*it)->applyDynamics();
        
        Point2d position = worldModel->getPosition();
        (*it)->oldPosition = position;
        
        if(!gUseOrganismLeader){
            Rx = Rx + worldModel->_agentAbsoluteLinearSpeed * cos(worldModel->_agentAbsoluteOrientation * M_PI / 180);
            Ry = Ry + worldModel->_agentAbsoluteLinearSpeed * sin(worldModel->_agentAbsoluteOrientation * M_PI / 180);
        }
    }
    
    if(gUseOrganismLeader){
        RobotAgentWorldModel* wm = leader->getWorldModel();
        Rx = wm->_agentAbsoluteLinearSpeed * cos(wm->_agentAbsoluteOrientation * M_PI / 180);
        Ry = wm->_agentAbsoluteLinearSpeed * sin(wm->_agentAbsoluteOrientation * M_PI / 180);
    }
	
    if (gRiverbed) {
		int riverAgents = this->agentsInRiverzone();
		double extraStrength = 0;
		if(riverAgents > 0){
			extraStrength = log(riverAgents);
			Rx *= riverAgents;
			Ry *= riverAgents;
		}
		
		Rx += extraStrength * gStreamStrength * cos(gStreamDirection * M_PI / 180);
		Ry += extraStrength * gStreamStrength * sin(gStreamDirection * M_PI / 180);
    }
        
    angle = atan2(Ry, Rx);
    translation = sqrt(Rx * Rx + Ry * Ry);
    
    if(gOrganismNormaliseSpeed){
        translation = translation / this->size();
    }
}

void Organism::moveOrganism(double angle, double translation) {
	double _xDeltaReal, _yDeltaReal;
	std::vector<RobotAgentPtr>::iterator it;

	_xDeltaReal = translation * cos(angle);
	_yDeltaReal = translation * sin(angle);
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel* wm = (*it)->getWorldModel();

		if ((*it)->isRegistered()) {
			(*it)->unregisterAgent();
		}
        
        Point2d position = wm->getPosition();

		position.x += _xDeltaReal;
		position.y += _yDeltaReal;
        
        wm->setPosition(position);

		(*it)->setCoord((int) position.x + 0.5, (int) position.y + 0.5);
	}
}

bool Organism::detectCollisions() {
	// Collision detection
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		if ((*it)->isCollision()) {
			return true;
		}
	}
	return false;
}

void Organism::rollbackMove() {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel *wm = (*it)->getWorldModel();
		wm->_desiredTranslationalValue = 0; // cancel any translation order as agent translation speed is set to zero after collision. (note that rotation is still ok)
		wm->_actualTranslationalValue = 0;
		wm->_agentAbsoluteLinearSpeed = 0;
        
        Point2d position = (*it)->oldPosition;
        wm->setPosition(position);
		(*it)->setCoord((int) position.x + 0.5, (int) position.y + 0.5);

		if (gLocomotionMode == 1) // consider obstacle friction or not for rotation?
		{
			wm->_agentAbsoluteOrientation = (*it)->_lastAgentAbsoluteOrientation;
			wm->_actualRotationalVelocity = 0;
		} else {
			wm->_actualRotationalVelocity = wm->_desiredRotationalVelocity;
		}
	}
}

// actual rotational and translational values matches desired values
void Organism::updateWorldModels() {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel *wm = (*it)->getWorldModel();
		wm->_actualRotationalVelocity = wm->_desiredRotationalVelocity;
		wm->_actualTranslationalValue = wm->_agentAbsoluteLinearSpeed;
	}
}

void Organism::updateSensors() {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel *wm = (*it)->getWorldModel();
        wm->updateSensors();
		// update sensors
		/*
		for (int i = 0; i != wm->getDefaultSensors()->getSensorCount(); i++) {
			// Warning: the following is repeated in the show method because coordinates are not stored, but are needed to display the sensor rays.
			double x1 = (wm->_xReal + wm->_sensors[i][1] * cos(wm->_sensors[i][2] + wm->_agentAbsoluteOrientation * M_PI / 180));
			double y1 = (wm->_yReal + wm->_sensors[i][1] * sin(wm->_sensors[i][2] + wm->_agentAbsoluteOrientation * M_PI / 180));
			double x2 = (wm->_xReal + wm->_sensors[i][3] * cos(wm->_sensors[i][4] + wm->_agentAbsoluteOrientation * M_PI / 180));
			double y2 = (wm->_yReal + wm->_sensors[i][3] * sin(wm->_sensors[i][4] + wm->_agentAbsoluteOrientation * M_PI / 180));

			// cast sensor ray.
			wm->_sensors[i][5] = castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, wm->getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object. -- not used here.
			Uint8 r, g, b;
			Uint32 pixel = getPixel32(gEnvironmentImage, x2, y2);
			SDL_GetRGB(pixel, gEnvironmentImage->format, &r, &g, &b);
			wm->_sensors[i][6] = (r << 16) + (g << 8) + b; // R=objects, B=agents, can only be one agent OR one object in each location.
		} //(old: (r<<16)+(g<<8)+b;)

		Uint8 r, g, b;
		Uint32 pixel = getPixel32(gZoneImage, wm->_xReal + 0.5, wm->_yReal + 0.5);
		SDL_GetRGB(pixel, gZoneImage->format, &r, &g, &b);
		wm->getDefaultSensors()->getFloorSensor() = r;
		*/
	}
}

void Organism::registerOrganism() {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		if (!(*it)->isRegistered()) {
			(*it)->registerAgent();
		}
	}
}

void Organism::getBaryCenter(double &x, double &y) {
	x = 0;
	y = 0;
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel* wm = (*it)->getWorldModel();
        Point2d position = wm->getPosition();
		x += position.x;
		y += position.y;
	}
	
	x = x / agents.size();
	y = y / agents.size();
}

int Organism::agentsInRiverzone() {
	int nAgents = 0;
	std::vector<RobotAgentPtr>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++) {
		RobotAgentWorldModel* wm = (*it)->getWorldModel();
		if(wm->getDefaultSensors()->getFloorSensor() == gRiverbedZone || gRiverbedZone == -1)
			nAgents++;
	}
	return nAgents;
}
