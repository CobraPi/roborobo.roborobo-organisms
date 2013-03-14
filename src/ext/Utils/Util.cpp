/*
 *  Util.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utils/Util.h"

Util::Util() {
	// nothing here
}

Util::~Util() {
	// nothing here
}

NeuralControllerPtr Util::createRandomGenome(RobotAgentWorldModel *__wm) {
	vector<double> weights = vector<double> (18, 0);
	for (int i = 0; i < 18; i++) {
		weights[i] = Rand::randouble() * 2.0 - 1;
	}
	return NeuralControllerPtr(new NeuralController(weights));
}

/*Controller *Util::binaryTournament(vector<Controller *> *candidates, Controller *exclude) {
 Controller *winner = NULL;

 // randomly choose two controllers
 Controller *candidate1 = NULL;
 while (candidate1 == NULL || candidate1 == exclude) {
 candidate1 = randomController(candidates);
 }

 Controller *candidate2 = NULL;
 while (candidate2 == NULL || candidate2 == exclude
 || candidate2 == candidate1) {
 candidate2 = randomController(candidates);
 }

 // pick controller with highest fitness value as winner
 winner = (candidate1->GetFitness() > candidate2->GetFitness() ? candidate1
 : candidate2);
 return winner;
 }*/

ControllerPtr Util::binaryTournament(vector<ControllerPtr> *candidates, vector<ControllerPtr> *exclude) {
	ControllerPtr winner;

	// randomly choose two controllers
	ControllerPtr candidate1 = randomController(candidates);
	if (exclude != NULL)
		while (checkExcludesForController(candidate1, exclude))
			candidate1 = randomController(candidates);
	ControllerPtr candidate2 = randomController(candidates);
	if (exclude != NULL) {
		while (checkExcludesForController(candidate2, exclude) || candidate2 == candidate1)
			candidate2 = randomController(candidates);
	} else {
		while (candidate2 == candidate1)
			candidate2 = randomController(candidates);
	}
	// pick controller with highest fitness value as winner
	winner = (candidate1->GetFitness() > candidate2->GetFitness() ? candidate1 : candidate2);
	return winner;
}

bool Util::checkExcludesForController(ControllerPtr xController, vector<ControllerPtr> *exclude) {
	if (exclude == NULL)
		return false;
	for (vector<ControllerPtr>::iterator it = exclude->begin(); it != exclude->end(); it++) {
		if (xController == *it) {
			return true;
		}
	}
	return false;
}

ControllerPtr Util::randomController(vector<ControllerPtr> *candidates) {
	return candidates->at((int) (Rand::randouble() * candidates->size()));
}

NeuralControllerPtr Util::avgCrossover(NeuralControllerPtr parent1, NeuralControllerPtr parent2) {
	NeuralControllerPtr result = boost::dynamic_pointer_cast<NeuralController>(parent1->Clone());
	for (int i = 0; i < 18; i++) {
		result->SetWeight(i, (parent1->GetWeight(i) + parent2->GetWeight(i)) / 2);
		result->SetMutationStepSize(i, (parent1->GetMutationStepSize(i) + parent2->GetMutationStepSize(i)) / 2);
	}
	return result;
}

NeuralControllerPtr Util::avgMultiParentCrossover(vector<NeuralControllerPtr> *parents) {
	NeuralControllerPtr result = boost::dynamic_pointer_cast<NeuralController>(parents->at(0)->Clone());
	int weights = boost::dynamic_pointer_cast<NeuralController>(parents->at(0))->GetWeights().size();
	double weight = 0;
	double stepSize = 0;
	for (int i = 0; i < weights; i++) {
		for (uint j = 0; j < parents->size(); j++) {
			weight += boost::dynamic_pointer_cast<NeuralController> (parents->at(j))->GetWeight(i);
			stepSize += boost::dynamic_pointer_cast<NeuralController> (parents->at(j))->GetMutationStepSize(i);
		}
		result->SetWeight(i, weight / parents->size());
		result->SetMutationStepSize(i, stepSize / parents->size());
	}
	return result;
}

NeuralControllerPtr Util::nPointMultiParentCrossover(vector<NeuralControllerPtr> *parents) {
	NeuralControllerPtr result = boost::dynamic_pointer_cast<NeuralController>(parents->at(0)->Clone());
	int weights = boost::dynamic_pointer_cast<NeuralController> (parents->at(0))->GetWeights().size();
	vector<int> points;
	points.push_back(0);
	for (uint i = 0; i < parents->size() - 1; i++) {
		points.push_back(Rand::randint(0, result->GetWeights().size()));
	}
	points.push_back(weights);
	std::sort(points.begin(), points.end());
	for (uint i = 1; i < parents->size(); i++) {
		for (int j = points.at(i - 1); j < points.at(i); j++) {
			result->SetWeight(j, boost::dynamic_pointer_cast<NeuralController> (parents->at(i - 1))->GetWeight(i));
			result->SetMutationStepSize(j, boost::dynamic_pointer_cast<NeuralController> (parents->at(j))->GetMutationStepSize(i));
		}
	}
	return result;
}

void Util::mutate(NeuralController *victim) {
	// mutate the sigma
	double normalDraw = Rand::gaussianf();

	for (int i = 0; i < 18; i++) {
		double old = victim->GetMutationStepSize(i);
		double normalDrawI = Rand::gaussianf();
		double t1 = 1 / sqrt(2 * 18);
		double t = 1 / sqrt(2 * sqrt(18));
		double fresh = old * exp(t1 * normalDraw + t * normalDrawI);
		victim->SetMutationStepSize(i, fresh);
	}

	// mutate the weights
	for (int i = 0; i < 18; i++) {
		double old = victim->GetWeight(i);
		double normalDrawI = Rand::gaussianf();
		double fresh = old + victim->GetMutationStepSize(i) * normalDrawI;
		victim->SetWeight(i, fresh);
	}
}

/**
 * Average all positive values in a vector; that is: a normal average of the vector, but
 * all negative values are ignored. 0 is not ignored.
 */
double Util::avgPositiveOnly(vector<double> *list) {
	double result = 0.0;
	int size = 0;

	for (unsigned int i = 0; i < list->size(); i++) {
		if (list->at(i) >= 0) {
			result += list->at(i);
			size++;
		}
	}
	if (size > 0) {
		result /= size;
	}

	return result;
}

bool Util::checkExcludesForItem(NewsItem *item, vector<ControllerPtr> *exclude) {
	if (exclude == NULL)
		return false;
	for (vector<ControllerPtr>::iterator it = exclude->begin(); it != exclude->end(); it++) {
		if (item->getContent() == *it)
			return true;
	}
	return false;
}
