#include <algorithm>
#include <limits>
#include <limits.h>

#include "Utils/Controller.h"
#include "Dynamic/include/DynamicAgentWorldModel.h"
#include "Dynamic/include/DynamicController.h"
#include "Dynamic/include/DynamicSharedData.h"
#include "World/World.h"
#include "Agents/Agent.h"
#include "Utils/Rand.h"

static int MAX_ORG_SIZE = 10;
static int MAX_SPEED = 1;
static int MAX_ANGLE = 180;

DynamicController::DynamicController(vector<double> weights) {
	this->weights = weights;
	this->neuralNet = NULL;
	this->mutationStepSizes = vector<double> (weights.size(), initialMutationStepSize);
}

DynamicController::DynamicController(vector<double> weights, vector<double> mutationStepSizes) :
	Controller(mutationStepSizes) {
	this->weights = weights;
	this->neuralNet = NULL;
}

DynamicController::~DynamicController() {
	if (this->neuralNet) {
		delete this->neuralNet;
		this->neuralNet = NULL;
	}
}

void DynamicController::reset() {
	DynamicSharedData::genomeLogFile << this->id << "," << gWorld->getIterations() << ",";
	for (uint i = 0; i < this->weights.size(); i++) {
		DynamicSharedData::genomeLogFile << this->weights[i] << ":";
	}
	DynamicSharedData::genomeLogFile << std::endl;

	desiredAngle = -270;

	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	worldModel->penalty = 0;
    worldModel->xStart = worldModel->_xReal;
    worldModel->yStart = worldModel->_yReal;
    
	worldModel->setEnergyLevel(fmin(worldModel->getEnergyLevel()+DynamicSharedData::START_ENERGY,DynamicSharedData::MAX_ENERGY));
	worldModel->setEnergyGained(0);
	worldModel->setDeltaEnergy(0);
	worldModel->setActive(true);
}

void DynamicController::step(double &left, double &right) {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);

    if(worldModel->getEnergyLevel() <= 0){
        return;
    }
    
	setLED();
	agent->setConnectToOthers(Agent::NEUTRAL);

	if(agent->isPartOfOrganism()){
		worldModel->organismTime++;
	}else{
		worldModel->swarmTime++;
	}

	// retrieve the values of the distance sensors
	vector<double> inputs = this->getSensorValues();
	if (inputs.size() == 0) {
		cerr << "ERR: robot did not return any distance sensor data - can't build neural net" << endl;
		return;
	}

	// Outputs are the following:
    // Node 1: Form Organism
    // Node 2: Leave Organism
    // Node 3: Halt
    // Node 4: Avoid obstacle
    // Node 5: Move to direction
    // Node 6: Desired organism size - for node 1
    // Node 7: Desired direction - for node 5
    // Node 8: Desired speed - for node 5

	vector<double> outputs = vector<double> (8, 0);

	// create the neural net if it didn't exist already
	if (neuralNet == NULL) {
		neuralNet = new SimplePerceptron(inputs.size(), outputs.size());
		neuralNet->setActivationFunction(&ActivationFunctionTanh::apply);

		// load the weights
		neuralNet->loadParameters(&weights.front(), weights.size());
	}

	// set the sensor values as input neuron values
	neuralNet->step(&inputs.front());

	// and calculate the output
	neuralNet->getOutputValues(&outputs.front());

	vector<double> choices;
	// first 5 elements are strategies
	for (int i = 0; i < 5; i++) {
		choices.push_back(outputs[i]);
	}

	// the first 5 are interpreted as a choice for a strategy. Highest wins.
	int choice = distance(choices.begin(), max_element(choices.begin(), choices.end()));

	// Obtain the numbers for speed, direction, and size of the organism.
    int org_size = (int) MAX_ORG_SIZE * ((outputs[5]+1)/2);
    double direction = MAX_ANGLE * outputs[6];
    double speed = MAX_SPEED * ((outputs[7]+1)/2);

    // Do some logging
    if (gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		DynamicSharedData::outputLogFile << worldModel->_agentId << "," << gWorld->getIterations() << ", ";
		for (int i = 0; i < 8; i++) {
			DynamicSharedData::outputLogFile << outputs[i] << ((i < 7) ? ":" : "");
		}
		DynamicSharedData::outputLogFile << "," << choice << "," << org_size << "," << direction << "," << speed << std::endl;
	}

    /** For Test Purposes */
//    org_size = 10;
//    speed = 1;
//    direction = 0;
//    if(!agent->isPartOfOrganism() || agent->getOrganism()->size() < 3){
//        choice = FORM;
//    }else{
//        choice = MOVE;
//    }

	//std::cout << "Robot: " << worldModel->_agentId << " is ";
    
    switch (choice) {
		case FORM:
			//std::cout << "forming organism of size " << org_size << " with speed " << speed;
			createOrganism(left, right, org_size, speed);
			break;
		case LEAVE:
			//std::cout << "leaving organism";
			leaveOrganism(left, right);
			break;
		case HALT:
			//std::cout << "halting";
			//halt(left, right);
            move(left, right, direction, speed);
			break;
		case AVOID:
			//std::cout << "avoiding obstacles with speed " << speed;
			avoidObstacles(left, right, speed);
			break;
		case MOVE:
			//std::cout << "moving with speed " << speed << " in the direction " << direction;
			move(left, right, direction, speed);
			break;
	}
	//std::cout << std::endl;
}


void DynamicController::createOrganism(double &left, double &right, int desired_size, double speed) {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);

    // You are only eager to connect when your current organism is not large enough according to your standards.
    if (agent->isPartOfOrganism()){
        Organism *o = agent->getOrganism();

        if (o->size() < desired_size){
            agent->setConnectToOthers(Agent::POSITIVE);
        }else{
            agent->setConnectToOthers(Agent::NEUTRAL);
        }
    }else{
        agent->setConnectToOthers(Agent::POSITIVE);
    }

	if (agent->getConnectToOthers() == Agent::NEGATIVE) {
		avoidObstacles(left, right, speed);
		return;
	}

	Point2d posRobot(worldModel->_xReal, worldModel->_yReal);

	double closestDistance = getMaximumDistance();
	bool found = false;
	Point2d closestPoint;
	RobotAgentPtr closest;

	vector<RobotAgentPtr> close = agent->getNearRobots();

	// find robots that we can connect to
	vector<RobotAgentPtr>::iterator it;
	for (it = close.begin(); it != close.end(); it++) {
		if ((*it)->getConnectToOthers() == Agent::POSITIVE) {
			Point2d closeRobot((*it)->getWorldModel()->_xReal, (*it)->getWorldModel()->_yReal);
			double distance = getEuclidianDistance(posRobot, closeRobot);
			if (distance < closestDistance) {
				found = true;
				closestDistance = distance;
				closestPoint = closeRobot;
				closest = (*it);
			}
		}
	}

	// No robots that want to connect are close
	if (!found) {
		avoidObstacles(left, right, speed);
	} else {
		// steer towards the closest robot that wants to connect
		double angle = (atan2(closestPoint.y - posRobot.y, closestPoint.x - posRobot.x) / M_PI) * 180.0;
		double diffAngle = angle - _wm->_agentAbsoluteOrientation;
		if (diffAngle < -180.0) {
			diffAngle += 360.0;
		}
		if (diffAngle > 180.0) {
			diffAngle -= 360.0;
		}

		followAngle(diffAngle, left, right);
		left *= speed;
		right *= speed;
	}
}

void DynamicController::leaveOrganism(double &left, double &right) {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);
    if (agent->isPartOfOrganism()){
        // Note: implicit knowledge here: leaving means no longer wanting to connect until create organism is called again.
        agent->setConnectToOthers(Agent::NEGATIVE);
        agent->letGoOfOrganism();
    }
}

void DynamicController::halt(double &left, double &right){
    // Just set the left and right wheel to speed 0
    left = 0;
    right = 0;
}

void DynamicController::move(double &left, double &right, double direction, double speed){
    double diffAngle = direction - _wm->_agentAbsoluteOrientation;
	if (diffAngle < -180.0) {
		diffAngle += 360.0;
	}
	if (diffAngle > 180.0) {
		diffAngle -= 360.0;
	}
    // The speed factor should still be included.
    followAngle(diffAngle, left, right);
    // Set the speed to an appropriate value, assuming that followAngle provides the top speed.
    left = left * speed;
    right = right * speed;
}

vector<double> DynamicController::getSensorValues() {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);

    /* Inputs:
    * Distance Sensors x5
    * Energy Sensors x3
    * Energy Value Sensor x3
    * organism size
    * energy level
    * 1 bias node
    */
    
	vector<double> sensors;
	for (int i = 0; i < 5; i++) {
		sensors.push_back( _wm->getDefaultSensors()->getSensorDistanceValue(i) / _wm->getDefaultSensors()->getSensorMaximumDistanceValue(i));
	}
    
    for(int i=1;i<4;i++){
		sensors.push_back(_wm->getDefaultSensors()->getEnergyDistanceValue(i) / _wm->getDefaultSensors()->getSensorMaximumDistanceValue(i));
    	sensors.push_back(_wm->getDefaultSensors()->getEnergyLevelValue(i) / DynamicSharedData::NUM_WEIGHTS);
    }

    // Organism Size
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);
	if (agent->getOrganism()) {
		sensors.push_back(agent->getOrganism()->size() / gWorld->getNbOfAgent());
	} else {
		sensors.push_back(0.0);
	}
    
    // Energy Level
    sensors.push_back(worldModel->getEnergyLevel() / DynamicSharedData::MAX_ENERGY);

    // bias node
    sensors.push_back(1.0);
    
    /**
     * Make sure the inputs are correctly normalised
     */
    for(int i=0;i< sensors.size(); i++){
        assert(sensors[i] <= 1.0 && sensors[i] >= -1.0);
    }
    
	return sensors;
}

void DynamicController::randomWalk(double &left, double &right, double speed) {
	if (desiredAngle < -180) {
		desiredAngle = ((Rand::randouble() * 2) - 1) * 180;
	}

	double diffAngle = desiredAngle - _wm->_agentAbsoluteOrientation;
	if (diffAngle < -180.0) {
		diffAngle += 360.0;
	}
	if (diffAngle > 180.0) {
		diffAngle -= 360.0;
	}

	followAngle(diffAngle, left, right);
	left *= speed;
	right *= speed;
}

void DynamicController::avoidObstacles(double &left, double &right, double speed) {
	_wm->getDefaultSensors()->getSensorDistanceValue(2);
	left= 1 - ( (double)gSensorRange - ((_wm->getDefaultSensors()->getSensorDistanceValue(2)+_wm->getDefaultSensors()->getSensorDistanceValue(3))/2) )  / (double)gSensorRange;;
    right =  1 - ( (double)gSensorRange - ((_wm->getDefaultSensors()->getSensorDistanceValue(2)+_wm->getDefaultSensors()->getSensorDistanceValue(3))/2) )  / (double)gSensorRange;
	if ( _wm->getDefaultSensors()->getSensorDistanceValue(0) + _wm->getDefaultSensors()->getSensorDistanceValue(1) + _wm->getDefaultSensors()->getSensorDistanceValue(2) < _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) )
		right -= 0.2;
    else if (_wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) < 3*gSensorRange )
		left -= 0.2;
    else {
        if ( (double)(rand()%10)/10.0 < 0.5) {
            right -= (double)(rand()%10)/10.*0.2;
        }else{
            left -= (double)(rand()%10)/10.*0.2;
        }
    }
    
    left *= speed;
    right *= speed;
}

ControllerPtr DynamicController::Clone() {
    ControllerPtr p = boost::make_shared<DynamicController>(weights, mutationStepSizes);
    return p;
}

ControllerPtr DynamicController::crossOverWith(ControllerPtr partner) {
	DynamicControllerPtr result = boost::dynamic_pointer_cast<DynamicController>(this->Clone());
	DynamicControllerPtr other = boost::dynamic_pointer_cast<DynamicController>(partner);
	for (size_t i = 0; i < this->weights.size(); i++) {
		result->weights[i] = (this->weights[i] + other->weights[i]) / 2;
		result->mutationStepSizes[i] = (this->mutationStepSizes[i] + other->mutationStepSizes[i]) / 2;
	}
	return boost::dynamic_pointer_cast<Controller>(result);
}

ControllerPtr DynamicController::crossOverWithMP(vector<ControllerPtr> *parents) {
	DynamicControllerPtr result = boost::dynamic_pointer_cast<DynamicController>(this->Clone());
	int weights = boost::dynamic_pointer_cast<DynamicController>(parents->at(0))->weights.size();
	for (int i = 0; i < weights; i++) {
		double weight = 0;
		double stepSize = 0;

		for (uint j = 0; j < parents->size(); j++) {
			weight += boost::dynamic_pointer_cast<DynamicController>(parents->at(j))->weights[i];
			stepSize += boost::dynamic_pointer_cast<DynamicController>(parents->at(j))->mutationStepSizes[i];
		}

		result->weights[i] = weight / parents->size();
		result->mutationStepSizes[i] = stepSize / parents->size();
	}
	return boost::dynamic_pointer_cast<Controller>(result);
}

void DynamicController::mutate() {
	// Draw a global number
	// Uncorrelated  mutation with n sigma's
	double normalDraw = Rand::gaussianf();

	for (uint i = 0; i < this->mutationStepSizes.size(); i++) {
		double old = this->mutationStepSizes[i];
		double normalDrawI = Rand::gaussianf();
		double t1 = 1 / sqrt(2 * this->mutationStepSizes.size());
		double t = 1 / sqrt(2 * sqrt(this->mutationStepSizes.size()));
		double fresh = old * exp(t1 * normalDraw + t * normalDrawI);

		if (fresh < controllerMutationStepSizeMinimum) {
			fresh = controllerMutationStepSizeMinimum;
		}

		this->mutationStepSizes[i] = fresh;
	}

	// mutate the weights
	for (uint i = 0; i < this->weights.size(); i++) {
		double old = this->weights[i];
		double normalDrawI = Rand::gaussianf();
		double fresh = old + this->mutationStepSizes[i] * normalDrawI;
		this->weights[i] = fresh;
	}
}

void DynamicController::followAngle(double angle, double &left, double &right) {
	left = right = 1;
	bool close = false;
	for (int i = 0; i < 6; i++) {
		if (_wm->getDefaultSensors()->getSensorDistanceValue(i) < 10) {
			close = true;
		}
	}

	if (close && _wm->getDefaultSensors()->getSensorDistanceValue(0) + _wm->getDefaultSensors()->getSensorDistanceValue(1) + _wm->getDefaultSensors()->getSensorDistanceValue(2) < _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5)) {
		right -= 0.4;
	} else if (close && _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) < 1.5 * gSensorRange) {
		left -= 0.4;
	} else if (angle > 5) {
		// turn right
		right -= 0.4;
	} else if (angle < -5) {
		// turn left
		left -= 0.4;
	} else if (angle > 0) {
		// speed up if you're on target
		right -= 0.01;
	} else if (angle < 0) {
		left -= 0.01;
	}
}

void DynamicController::setLED() {
	DynamicAgentWorldModel* worldModel = dynamic_cast<DynamicAgentWorldModel*> (_wm);
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);


	worldModel->setRobotLED_colorValues(0, 255, 0);
	if(agent->isPartOfOrganism() && agent->getOrganism()->leader == agent){
		worldModel->setRobotLED_status(true);
	}else{
		worldModel->setRobotLED_status(false);
	}
}

std::string DynamicController::ToString(){
	ostringstream os;
	os << "controller ";
	os << id;
	os << endl << "w: ";
	for (unsigned int i = 0; i < weights.size(); i++) {
		if (i > 0) {
			os << ", ";
		}
		os << weights[i];
	}

	os << endl << "m: ";
	for (unsigned int i = 0; i < mutationStepSizes.size(); i++) {
		if (i > 0) {
			os << ", ";
		}
		os << mutationStepSizes[i];
	}

	return os.str();
}
