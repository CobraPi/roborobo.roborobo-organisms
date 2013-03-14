/* 
 * File:   ScalePowerSimpleController.h
 * Author: bweel
 *
 * Created on May 23, 2011, 10:58 AM
 */

#ifndef ScalePowerSimpleCONTROLLER_H
#define	ScalePowerSimpleCONTROLLER_H

#include "Utils/Controller.h"
#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

class ScalePowerSimpleController : public Controller {
public:
	ScalePowerSimpleController(vector<double> genome);
	ScalePowerSimpleController(vector<double> genome, vector<double> mutationStepSizes);
	
	virtual void step(double &left, double &right);
	virtual void reset();
	virtual ControllerPtr Clone();
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();

private:
	double desiredAngle; // used for random walk
	
	static const bool debug = false;
	
	void searchClosestEnergyPoint();
	void setLED();
	void avoidWalls(double &left, double &right);
	void goToFeedingGround(double &left, double &right);
	
	vector<double> genome;
};


#endif	/* ScalePowerSimpleCONTROLLER_H */

