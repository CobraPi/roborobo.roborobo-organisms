#include <algorithm>
#include <limits>
#include <limits.h>

#include "Utils/Controller.h"
#include "ScalePowerSimple/include/ScalePowerSimpleAgentWorldModel.h"
#include "ScalePowerSimple/include/ScalePowerSimpleController.h"
#include "ScalePowerSimple/include/ScalePowerSimpleSharedData.h"
#include "World/World.h"
#include "Agents/Agent.h"
#include "Utils/Rand.h"

ScalePowerSimpleController::ScalePowerSimpleController(std::vector<double> genome) {
	this->genome = genome;
}

ScalePowerSimpleController::ScalePowerSimpleController(std::vector<double> genome, std::vector<double> mutationStepSizes) : Controller(mutationStepSizes) {
	this->genome = genome;
}

void ScalePowerSimpleController::step(double &left, double &right) {
	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);

	setLED();

	if (!worldModel->isActive()) {
		return;
	}
	
	//left += 1 - ((double) gSensorRange - ((worldModel->_sensors[2][5] + worldModel->_sensors[3][5]) / 2)) / (double) gSensorRange;
	//right += 1 - ((double) gSensorRange - ((worldModel->_sensors[2][5] + worldModel->_sensors[3][5]) / 2)) / (double) gSensorRange;

	avoidWalls(left,right);

	goToFeedingGround(left,right);
}

void ScalePowerSimpleController::reset() {
	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);

	
	worldModel->setEnergyLevel(ScalePowerSimpleSharedData::START_ENERGY);
	worldModel->setEnergyGained(0);
	worldModel->setDeltaEnergy(0);
	worldModel->setActive(true);
	
	ScalePowerSimpleSharedData::genomeLogFile << worldModel->_agentId << "," <<  gWorld->getIterations() << "," << genome[0] <<":" << genome[1] << std::endl;
	
	double random = Rand::randouble();
	if (random < genome[0]) {
		gWorld->getAgent(worldModel->_agentId)->setConnectToOthers(Agent::POSITIVE);
	} else if (random < genome[1]) {
		gWorld->getAgent(worldModel->_agentId)->setConnectToOthers(Agent::NEGATIVE);
	} else {
		gWorld->getAgent(worldModel->_agentId)->setConnectToOthers(Agent::NEUTRAL);
	}
	
	ScalePowerSimpleSharedData::outputLogFile << worldModel->_agentId << "," << gWorld->getIterations() << "," << gWorld->getAgent(worldModel->_agentId)->getConnectToOthers() << std::endl;
}

void ScalePowerSimpleController::searchClosestEnergyPoint() {
	double angleToClosestEnergyPoint = 0.0;
	double shortestDistance = 0.0;

	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);

	//search the current active energy point

	Point2d posRobot(worldModel->_xReal, worldModel->_yReal);
	Point2d closestPoint = gEnergyPoints[ScalePowerSimpleSharedData::gActiveSun].getPosition();

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

void ScalePowerSimpleController::setLED() {
	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);
	if (worldModel->getDeltaEnergy() > 0) {
		worldModel->setRobotLED_colorValues(0, 255, 0); // green = gaining
	} else if (worldModel->getDeltaEnergy() < 0) {
		worldModel->setRobotLED_colorValues(255, 0, 0); // red = depleting
	} else {
		worldModel->setRobotLED_colorValues(255, 150, 0); // orange = stable
	}
	worldModel->setRobotLED_status(true);
}

void ScalePowerSimpleController::avoidWalls(double &left, double &right) {
//	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);
//
//	if (worldModel->_sensors[0][5] + worldModel->_sensors[1][5] + worldModel->_sensors[2][5] < worldModel->_sensors[3][5] + worldModel->_sensors[4][5] + worldModel->_sensors[5][5]) {
//		left += 0.25;
//		right -= 0.25;
//	} else if (worldModel->_sensors[3][5] + worldModel->_sensors[4][5] + worldModel->_sensors[5][5] < 1.5 * gSensorRange) {
//		left -= 0.25;
//		right += 0.25;
//	}
}

void ScalePowerSimpleController::goToFeedingGround(double &left, double &right){
//	ScalePowerSimpleAgentWorldModel* worldModel = dynamic_cast<ScalePowerSimpleAgentWorldModel*> (_wm);
	
//	bool close = false;
	//for (int i = 0; i < 6; i++) {
	//	if (worldModel->_sensors[i][5] < 10) {
	//		close = true;
	//	}
	//}
	
	searchClosestEnergyPoint();
	
	//cout << "Angle to closest energy: " << worldModel->getAngleToClosestEnergyPoint() << std::endl;
	
	//if (close && worldModel->_sensors[0][5] + worldModel->_sensors[1][5] + worldModel->_sensors[2][5] < worldModel->_sensors[3][5] + worldModel->_sensors[4][5] + worldModel->_sensors[5][5]) {
	//	left += 0.25;
	//	right -= 0.25;
	//} else if (close && worldModel->_sensors[3][5] + worldModel->_sensors[4][5] + worldModel->_sensors[5][5] < 1.5 * gSensorRange) {
	//	left -= 0.25;
	//	right += 0.25;
	//} else if (worldModel->getAngleToClosestEnergyPoint() > 5) {
		// turn right
	//	left += 0.25;
	//	right -= 0.25;
	//} else if (worldModel->getAngleToClosestEnergyPoint() < -5) {
		// turn left
	//	right += 0.25;
	/*	left -= 0.25;
	} else if (worldModel->getAngleToClosestEnergyPoint() > 0) {
		// speed up if you're on target
		left += 0.100001;
		right += 0.1;
	} else if (worldModel->getAngleToClosestEnergyPoint() < 0) {
		left += 0.1;
		right += 0.100001;
	} else if (worldModel->getDistanceToClosestEnergyPoint() < gSensorRange) {
		left -= 0.1;
		right -= 0.1;
	} else {
		left += 0.2;
		right += 0.2;
	}
	*/
}


ControllerPtr ScalePowerSimpleController::Clone() {
	return ControllerPtr(new ScalePowerSimpleController(genome));
}

ControllerPtr ScalePowerSimpleController::crossOverWith(ControllerPtr partner) {
	//std::cout << "Doing a standard crossover" << std::endl;
	vector<double> genes = vector<double>(6);
	ScalePowerSimpleController *o = dynamic_cast<ScalePowerSimpleController*>(partner.get());
	for(unsigned int i=0;i<genome.size();i++){
		genes[i] = (genome[i] + o->genome[i])/2;
	}
	return ControllerPtr(new ScalePowerSimpleController(genes));
}

ControllerPtr ScalePowerSimpleController::crossOverWithMP(vector<ControllerPtr> *parents) {
	//std::cout << "Doing a multiparent crossover" << std::endl;
	vector<double> genes = vector<double>(6);
	for(unsigned int i=0;i<genome.size();i++){
		double gene = genome[i];
		vector<ControllerPtr>::iterator it;
		for(it = parents->begin();it != parents->end();it++){
			ScalePowerSimpleController *o = dynamic_cast<ScalePowerSimpleController *>((*it).get());
			gene += o->genome[i];
		}
		gene /= (parents->size() +1);

		genes[i] = gene;
	}
	
	return ControllerPtr(new ScalePowerSimpleController(genes));
}

void ScalePowerSimpleController::mutate() {
	//gLogFile << "Mutating genome" << std::endl;
	
	// update the genome with a random number between -1 and 1
	genome[0] += (Rand::randouble() *2) -1;
	if(genome[0] > 1){
		genome[0] = 1;
	}else if(genome[0] < 0){
		genome[0] = 0;
	}
	genome[1] += (Rand::randouble() *2) -1;
	if(genome[1] > 1){
		genome[1] = 1;
	}else if(genome[1] < 0){
		genome[1] = 0;
	}
}
