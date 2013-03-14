/*
 *  Controller.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * Controller.cc
 *
 *  Created on: Jun 9, 2010
 *      Author: rjh
 */

#include "Utils/NeuralController.h"
#include "Utils/Util.h"

#include "neuralnets/SimplePerceptron.h"
#include "activationfunctions/ActivationFunctionTanh.h"

NeuralController::NeuralController(vector<LIOReal> weights) {
	this->weights = weights;
	this->neuralNet = NULL;
	this->mutationStepSizes = vector<double> (weights.size(), initialMutationStepSize);
}

NeuralController::NeuralController(vector<LIOReal> weights, vector<double> mutationStepSizes) :
	Controller(mutationStepSizes) {
	this->weights = weights;
	this->neuralNet = NULL;
}

NeuralController::~NeuralController() {
	// TODO Auto-generated destructor stub
}

vector<LIOReal> NeuralController::GetWeights() {
	return weights;
}

double NeuralController::GetWeight(int i) {
	return this->weights[i];
}

void NeuralController::SetWeight(int i, double w) {
	this->weights[i] = w;
}

void NeuralController::step(double &left, double &right) {

	// retrieve the values of the distance sensors
	vector<LIOReal>* distances = this->getSensorValues();
	if (distances->size() == 0) {
		cerr << "ERR: robot did not return any distance sensor data - can't build neural net" << endl;
		return;
	}
	distances->push_back(1); // bias node
	vector<LIOReal>* actuatorValues = this->getOutputTargetVector();

	// create the neural net if it didn't exist already
	if (neuralNet == NULL) {
		neuralNet = new SimplePerceptron(distances->size(), actuatorValues->size());
		neuralNet->setActivationFunction(&ActivationFunctionTanh::apply);
	}

	// have the subclass compute the weights for the neural net
	vector<LIOReal> nwWeights = this->GetWeights();
	if (nwWeights.size() == 0) { // don't do anything if an empty vector was returned
		return;
	}

	// update the neural net with the just-computed values
	neuralNet->loadParameters(&nwWeights.front(), nwWeights.size());

	// set the sensor values as input neuron values
	neuralNet->step(&distances->front());
	neuralNet->getOutputValues(&actuatorValues->front());

	// use the output neuron values to steer the robot
	//vector<double> output = neuralNet->getOutputs();
	this->useOutputs((*actuatorValues), left, right);

	delete distances;
	delete actuatorValues;
}

void NeuralController::reset() {
	// nothing to do;
}

vector<LIOReal>* NeuralController::getOutputTargetVector() {
	return new vector<LIOReal> (2);
}

vector<double>* NeuralController::getSensorValues() {
	vector<double>* sensors = new vector<double>();
	int i;
	for (i = 0; i < _wm->getDefaultSensors()->getSensorCount(); i++) {
		sensors->push_back(_wm->getDefaultSensors()->getSensorDistanceValue(i) / _wm->getDefaultSensors()->getSensorMaximumDistanceValue(i));
	}
	return sensors;
}

void NeuralController::useOutputs(vector<LIOReal> output, double &left, double &right) {
	left = output[0];
	right = output[1];
}

ControllerPtr NeuralController::Clone() {
	NeuralControllerPtr clone(new NeuralController(weights, mutationStepSizes));
	clone->SetOriginalFitness(this->GetOriginalFitness());
	return boost::dynamic_pointer_cast<Controller>(clone);
}

ControllerPtr NeuralController::crossOverWith(ControllerPtr partner) {
	return (Util::avgCrossover(shared_from_this(), boost::dynamic_pointer_cast<NeuralController>(partner)));
}

ControllerPtr NeuralController::crossOverWithMP(vector<ControllerPtr> *parents) {
	vector<NeuralControllerPtr> partners;
	for(size_t i=0;i<parents->size();i++){
		partners.push_back(boost::dynamic_pointer_cast<NeuralController>(parents->at(i)));
	}
	return boost::dynamic_pointer_cast<Controller>(Util::avgMultiParentCrossover(&partners));
}

void NeuralController::mutate() {
	Util::mutate(this);
}

string NeuralController::ToString() {
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

string NeuralController::ToShortString() {
	ostringstream os;
	os << "controller ";
	os << id;
	return os.str();
}

double NeuralController::calculateDifference(ControllerPtr other){
	NeuralControllerPtr na = shared_from_this();
	NeuralControllerPtr nb = boost::dynamic_pointer_cast<NeuralController>(other);
	
	vector<double> vecA = na->GetWeights();
	vector<double> vecB = nb->GetWeights();
	
	double difference = 0.0;
	for(unsigned int i = 0; i<vecA.size();i++){
		difference += abs(vecA.at(i) - vecB.at(i));
	}
	
	return difference/vecA.size();
}
