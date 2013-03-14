/* 
 * File:   ParcoursController.h
 * Author: bweel
 *
 * Created on May 23, 2011, 10:58 AM
 */

#ifndef ParcoursCONTROLLER_H
#define	ParcoursCONTROLLER_H

#include "Utils/Controller.h"
#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

class ParcoursController : public Controller, public boost::enable_shared_from_this<ParcoursController> {
	friend class ParcoursControlArchitecture;
public:
	ParcoursController(vector<double> weights);
	ParcoursController(vector<double> weights, vector<double> mutationStepSizes);
	virtual ~ParcoursController();
	
	virtual void step(double &left, double &right);
	virtual void reset();
	virtual ControllerPtr Clone();
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();
	
	virtual std::string ToString();

private:
	double desiredAngle; // used for random walk
	
	static const bool debug = false;

	void setLED();

	void createOrganism(double &left, double &right, int desired_size, double speed);
    void leaveOrganism(double &left, double &right);
    void halt(double &left, double &right);
    void move(double &left, double &right, double direction, double speed);
	void followAngle(double angle, double &left, double &right);
	void randomWalk(double &left, double &right, double speed);
    
    void setConnectToOthers(int connect);

	vector<double> getSensorValues();
	
	vector<double> weights;
	SimplePerceptron *neuralNet;
	
	enum Strategies {FORM, LEAVE, HALT, AVOID, MOVE};
};

typedef boost::shared_ptr<ParcoursController> ParcoursControllerPtr;

#endif	/* ParcoursCONTROLLER_H */

