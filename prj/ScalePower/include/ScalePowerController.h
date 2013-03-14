/* 
 * File:   ScalePowerController.h
 * Author: bweel
 *
 * Created on May 23, 2011, 10:58 AM
 */

#ifndef SCALEPOWERCONTROLLER_H
#define	SCALEPOWERCONTROLLER_H

#include "Utils/Controller.h"
#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

class ScalePowerController : public Controller {
public:
	ScalePowerController(vector<double> weights);
	ScalePowerController(vector<double> weights, vector<double> mutationStepSizes);
	virtual ~ScalePowerController();
	
	virtual void step(double &left, double &right);
	virtual void reset();
	virtual ControllerPtr Clone();
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();

private:
	double desiredAngle; // used for random walk
	
	static const bool debug = false;

	void setLED();

	void createOrganism(double &left, double &right, int desired_size);
    void leaveOrganism(double &left, double &right);
    void halt(double &left, double &right);
	void avoidObstacles(double &left, double &right);
    void move(double &left, double &right, double direction, double speed);
	void followAngle(double angle, double &left, double &right);
	void randomWalk(double &left, double &right);

	vector<double> getSensorValues();
	
	vector<double> weights;
	SimplePerceptron *neuralNet;
	
	enum Strategies {FORM, LEAVE, HALT, AVOID, MOVE};
	enum MagnetSettings {NEGATIVE,NEUTRAL,POSITIVE};
};

typedef boost::shared_ptr<ScalePowerController> ScalePowerControllerPtr;

#endif	/* SCALEPOWERCONTROLLER_H */

