/*
 *  Controller.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEURALCONTROLLER_H_
#define NEURALCONTROLLER_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "lioutils/lio_global.h"
#include "Utils/Controller.h"

#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"


using namespace std;

class NeuralController;
typedef boost::shared_ptr<NeuralController> NeuralControllerPtr;

class NeuralController : public Controller, public boost::enable_shared_from_this<NeuralController> {
public:
	NeuralController(vector<LIOReal> weights);
	NeuralController(vector<LIOReal> weights, vector<double> mutationStepSizes);
	virtual ~NeuralController();
	
	virtual vector<LIOReal> GetWeights();
	virtual double GetWeight(int i);
	virtual void SetWeight(int i, double w);
	virtual vector<double>* getSensorValues();
	virtual vector<LIOReal>* getOutputTargetVector();
	virtual void useOutputs(vector<LIOReal> output, double &left, double &right);
	
	virtual double calculateDifference(ControllerPtr other);
	
	virtual void step(double &left, double &right);
	virtual void reset();
	
	virtual ControllerPtr crossOverWith(ControllerPtr partner);
	virtual ControllerPtr crossOverWithMP(vector<ControllerPtr> *parents);
	virtual void mutate();
	
	virtual ControllerPtr Clone();
	virtual string ToString();
	virtual string ToShortString();

private:
	vector<LIOReal> weights;
	SimplePerceptron *neuralNet;
};

#endif /* NEURALCONTROLLER_H_ */
