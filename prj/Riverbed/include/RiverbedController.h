/* 
 * File:   ScalePowerController.h
 * Author: bweel
 *
 * Created on May 23, 2011, 10:58 AM
 */

#ifndef SCALEPOWERCONTROLLER_H
#define	SCALEPOWERCONTROLLER_H

#include "Utils/Controller.h"
#include "neuralnets/MultiLayeredPerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

class RiverbedController : public Controller {
public:
	RiverbedController(vector<double> weights);
	RiverbedController(vector<double> weights, vector<double> mutationStepSizes);
	virtual ~RiverbedController();
	
	virtual void step(double &left, double &right);
	virtual void reset();
	virtual ControllerPtr Clone();
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();

private:
	static const bool debug = false;
	
	void searchClosestEnergyPoint();
	void setLED(RobotAgentPtr agent);
	void randomWalk(double &left,double &right);
	void avoidObstacles(double &left, double &right);
	void createOrganism(double &left, double &right);
	void goToFeedingGround(double &left, double &right);
	
	void followAngle(double angle, double &left, double &right);
	
	vector<double> getSensorValues();
	
	vector<double> weights;
	MultiLayeredPerceptron *neuralNet;
	
	enum Strategies {RANDOM,AVOID,CREATE,FEED};
};

typedef boost::shared_ptr<RiverbedController> RiverbedControllerPtr;

#endif	/* SCALEPOWERCONTROLLER_H */

