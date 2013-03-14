/* 
 * File:   ChangeDetectionController.h
 * Author: bweel
 *
 * Created on May 23, 2011, 10:58 AM
 */

#ifndef ChangeDetectionCONTROLLER_H
#define	ChangeDetectionCONTROLLER_H

#include "multikulti/MultiKultiController.h"
#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

class ChangeDetectionController : public MultiKultiController, public boost::enable_shared_from_this<ChangeDetectionController> {
	friend class ChangeDetectionControlArchitecture;
public:
	ChangeDetectionController(vector<double> weights);
	ChangeDetectionController(vector<double> weights, vector<double> mutationStepSizes);
	virtual ~ChangeDetectionController();
	
	virtual void step(double &left, double &right);
	virtual void reset();
	virtual ControllerPtr Clone();
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();
	
	virtual double calculateDifference(MultiKultiControllerPtr other);
	
	virtual std::string ToString();

private:
	double desiredAngle; // used for random walk
	
	static const bool debug = false;

	void setLED();

	void createOrganism(double &left, double &right, int desired_size, double speed);
    void leaveOrganism(double &left, double &right);
    void halt(double &left, double &right);
	void avoidObstacles(double &left, double &right, double speed);
    void move(double &left, double &right, double direction, double speed);
	void followAngle(double angle, double &left, double &right);
	void randomWalk(double &left, double &right, double speed);

	vector<double> getSensorValues();
	
	vector<double> weights;
	SimplePerceptron *neuralNet;
	
	enum Strategies {FORM, LEAVE, HALT, AVOID, MOVE};
	enum MagnetSettings {NEGATIVE,NEUTRAL,POSITIVE};
};

typedef boost::shared_ptr<ChangeDetectionController> ChangeDetectionControllerPtr;

#endif	/* ChangeDetectionCONTROLLER_H */

