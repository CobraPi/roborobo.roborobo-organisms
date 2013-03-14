#include <algorithm>
#include <limits>
#include <limits.h>

#include "Utils/Controller.h"
#include "Riverbed/include/RiverbedAgentWorldModel.h"
#include "Riverbed/include/RiverbedController.h"
#include "Riverbed/include/RiverbedSharedData.h"
#include "World/World.h"
#include "Agents/Agent.h"
#include "Utils/Rand.h"

RiverbedController::RiverbedController(vector<double> weights) {
	this->weights = weights;
	this->neuralNet = NULL;
	this->mutationStepSizes = vector<double>(1, initialMutationStepSize);
}

RiverbedController::RiverbedController(vector<double> weights, vector<double> mutationStepSizes) : Controller(mutationStepSizes) {
	this->weights = weights;
	this->neuralNet = NULL;
}

RiverbedController::~RiverbedController() {
	if(this->neuralNet) {
		delete this->neuralNet;
		this->neuralNet = NULL;
	}
}

void RiverbedController::reset() {
	RiverbedAgentWorldModel* worldModel = dynamic_cast<RiverbedAgentWorldModel*> (_wm);

	RiverbedSharedData::genomeLogFile << this->id << "," << gWorld->getIterations() << ",";
	for(uint i=0;i<this->weights.size();i++){
		RiverbedSharedData::genomeLogFile << this->weights[i] << ":";
	}
	RiverbedSharedData::genomeLogFile << std::endl;

	worldModel->setEnergyLevel(RiverbedSharedData::START_ENERGY);
	worldModel->setEnergyGained(0);
	worldModel->setDeltaEnergy(0);
	worldModel->setActive(true);
}

void RiverbedController::step(double &left, double &right) {
	RiverbedAgentWorldModel* worldModel = dynamic_cast<RiverbedAgentWorldModel*> (_wm);
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);

	setLED(agent);

	if (!worldModel->isActive()) return;

	// retrieve the values of the distance sensors
	vector<double> inputs = this->getSensorValues();
	if (inputs.size() == 0) {
		cerr << "ERR: robot did not return any distance sensor data - can't build neural net" << endl;
		return;
	}
	inputs.push_back(1); // bias node

	// Output is a vote for 1 of 5 strategies.
	vector<double> outputs = vector<double>(5, 0);

	// create the neural net if it didn't exist already
	if (neuralNet == NULL) {
		neuralNet = new MultiLayeredPerceptron(inputs.size(), outputs.size(), 8);
		neuralNet->setActivationFunction(&ActivationFunctionTanh::apply,&ActivationFunctionTanh::apply);

		// load the weights
		neuralNet->loadParameters(&weights.front(), weights.size());
	}

	// set the sensor values as input neuron values
	neuralNet->step(&inputs.front());

	// and calculate the output
	neuralNet->getOutputValues(&outputs.front());

	if(gWorld->getIterations() % gOrganismSampleFrequency == 0) {
		RiverbedSharedData::outputLogFile << worldModel->_agentId << "," << gWorld->getIterations() << "," << outputs[0] << "," << outputs[1] << "," << outputs[2] << std::endl;
	}

	if (outputs[2] < -0.33) {
		agent->setConnectToOthers(Agent::NEGATIVE);
	} else if (outputs[2] > 0.33) {
		agent->setConnectToOthers(Agent::POSITIVE);
	} else {
		agent->setConnectToOthers(Agent::NEUTRAL);
	}

	left = right = outputs[0];
	double angle = outputs[1] * 180;
	followAngle(angle,left,right);
}

vector<double> RiverbedController::getSensorValues() {
	RiverbedAgentWorldModel* worldModel = dynamic_cast<RiverbedAgentWorldModel*> (_wm);

	vector<double> sensors;
	//for (int i = 0; i < worldModel->_sensorCount; i++) {
	//	sensors.push_back(worldModel->getSensorDistanceValue(i) / worldModel->getSensorMaximumDistanceValue(i));
	//}

	searchClosestEnergyPoint();
	sensors.push_back(worldModel->getAngleToClosestEnergyPoint() / 180);
	sensors.push_back(worldModel->getDistanceToClosestEnergyPoint() / getMaximumDistance());
	sensors.push_back(worldModel->getEnergyLevel() / INT_MAX);
	
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);
	if(agent->getOrganism()) {
		sensors.push_back(agent->getOrganism()->size() / gWorld->getNbOfAgent());
	} else {
		sensors.push_back(0);
	}

	return sensors;
}

ControllerPtr RiverbedController::Clone() {
	return ControllerPtr(new RiverbedController(weights, mutationStepSizes));
}

ControllerPtr RiverbedController::crossOverWith(ControllerPtr partner) {
	RiverbedControllerPtr result = boost::dynamic_pointer_cast<RiverbedController> (this->Clone());
	RiverbedControllerPtr other = boost::dynamic_pointer_cast<RiverbedController> (partner);

	result->mutationStepSizes[0] = (this->mutationStepSizes[0] + other->mutationStepSizes[0]) / 2;
	for (uint i = 0; i < this->weights.size(); i++) {
		result->weights[i] = (this->weights[i] + other->weights[i]) / 2;
	}
	return boost::dynamic_pointer_cast<Controller>(result);
}

ControllerPtr RiverbedController::crossOverWithMP(vector<ControllerPtr> *parents) {
	RiverbedControllerPtr result = boost::dynamic_pointer_cast<RiverbedController> (parents->at(0)->Clone());
	int weights = boost::dynamic_pointer_cast<RiverbedController> (parents->at(0))->weights.size();

	double stepSize = 0;
	for (uint j = 0; j < parents->size(); j++) {
		stepSize += boost::dynamic_pointer_cast<RiverbedController> (parents->at(j))->mutationStepSizes[0];
		result->mutationStepSizes[0] = stepSize / parents->size();
	}

	for (int i = 0; i < weights; i++) {
		double weight = 0;
		for (uint j = 0; j < parents->size(); j++) {
			weight += boost::dynamic_pointer_cast<RiverbedController> (parents->at(j))->weights[i];
		}
		result->weights[i] = weight / parents->size();
	}
	return boost::dynamic_pointer_cast<Controller>(result);;
}

void RiverbedController::mutate() {
	// mutate the sigma
	// Uncorrelated  mutation with n sigma's
	double normalDraw = Rand::gaussianf();

	for (uint i = 0; i < this->mutationStepSizes.size(); i++) {
		double old = this->mutationStepSizes[i];
		double normalDrawI = Rand::gaussianf();
		double t1 = 1 / sqrt(2 * this->mutationStepSizes.size());
		double t = 1 / sqrt(2 * sqrt(this->mutationStepSizes.size()));
		double fresh = old * exp(t1 * normalDraw + t * normalDrawI);

		if(fresh < controllerMutationStepSizeMinimum){
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

void RiverbedController::searchClosestEnergyPoint() {
	double angleToClosestEnergyPoint = 0.0;
	double shortestDistance = 0.0;

	RiverbedAgentWorldModel* worldModel = dynamic_cast<RiverbedAgentWorldModel*> (_wm);

	//search the current active energy point

	Point2d posRobot(worldModel->_xReal, worldModel->_yReal);
	Point2d closestPoint(0,worldModel->_yReal); // energy is better to the left of the robot, always.

	shortestDistance = getEuclidianDistance(posRobot, closestPoint);

	//compute the orientation of the active sun ( in degree between 0 and 360 )
	angleToClosestEnergyPoint = (atan2(closestPoint.y - posRobot.y, closestPoint.x - posRobot.x) / M_PI)*180.0;
	angleToClosestEnergyPoint += 360.0;
	angleToClosestEnergyPoint = computeModulo(angleToClosestEnergyPoint, 360.0);
	if (angleToClosestEnergyPoint > 180) { // force btw -180 and 180
		angleToClosestEnergyPoint -= 360.0;
	}

	//compute the angle between the actual orientation of the robot and the orientation of the closest energy point ( in degree between -180 and 180 )
	double diffAngleToClosestEnergyPoint = angleToClosestEnergyPoint - _wm->_agentAbsoluteOrientation;
	if (diffAngleToClosestEnergyPoint < -180.0) {
		diffAngleToClosestEnergyPoint += 360.0;
	}
	if (diffAngleToClosestEnergyPoint > 180.0) {
		diffAngleToClosestEnergyPoint -= 360.0;
	}

	if (debug) {
		std::cout << "Sun           : " << diffAngleToClosestEnergyPoint << "°, " << shortestDistance << "mm" << std::endl;
	}
	worldModel->setAngleToClosestEnergyPoint(diffAngleToClosestEnergyPoint);
	worldModel->setDistanceToClosestEnergyPoint(shortestDistance);

	if (debug) {
		std::cout << "SunSensorValue: " << worldModel->getAngleToClosestEnergyPoint() << "° , " << worldModel->getDistanceToClosestEnergyPoint() << "mm" << std::endl;
	}
}

void RiverbedController::followAngle(double angle, double &left, double &right) {
	double diffAngle = angle - _wm->_agentAbsoluteOrientation;
	if (diffAngle < -180.0) {
		diffAngle += 360.0;
	}
	if (diffAngle > 180.0) {
		diffAngle -= 360.0;
	}

	if (diffAngle > 5) {
		right -= 0.4;
	} else if (diffAngle < -5) {
		left -= 0.4;
	} else if (diffAngle > 0) {
		right -= 0.01;
	} else if (diffAngle < 0) {
		left -= 0.01;
	}
}

void RiverbedController::setLED(RobotAgentPtr agent) {

	if(agent->getConnectToOthers() == Agent::POSITIVE){
		_wm->setRobotLED_colorValues(0, 255, 0); // green = positive
	} else if (agent->getConnectToOthers() == Agent::NEGATIVE) {
		_wm->setRobotLED_colorValues(255, 0, 0); // red = negative
	} else {
		_wm->setRobotLED_colorValues(255, 150, 0); // orange = neutral
	}
	_wm->setRobotLED_status(true);
}
