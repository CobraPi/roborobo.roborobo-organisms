/*
 *  EvolutionaryControlArchitecture.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 06-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"
#include "Behavior/WallAvoidanceBehavior.h"

int EvolutionaryControlArchitecture::stepsPerCandidate = 500; // should be overwritten by properties file
EvolutionaryControlArchitecture::CalculationMethod EvolutionaryControlArchitecture::fitnessCalculation = AVERAGE;

EvolutionaryControlArchitecture::EvolutionaryControlArchitecture( RobotAgentWorldModel *__wm ) : DifferentialControlArchitecture ( __wm )
{
	this->phase = EVALUATING;
	this->stepsDone = -1;
	this->candidateFitness = 0;
	
	cout << "random seed: " << gRandomSeed << endl;
	Mathlib::setRandSeed(gRandomSeed);
	cout << "---- SIMULATION START ----" << endl;
}

EvolutionaryControlArchitecture::~EvolutionaryControlArchitecture()
{
	// nothing to do
	// candidate gets cleaned up elsewhere
}

void EvolutionaryControlArchitecture::step(){
	/* Handle the phase in which a robot controller is being evaluated */
	if (phase == EVALUATING) {
		if (stepsDone == 0) {
			if (debug) { cout << "Started evaluation of " << candidate->ToShortString() << endl; }
			candidate->reset();
		}
		
		/* Update the statistics about any possible running evaluation */
		if (stepsDone >= 0) {
			// determine the fitness during the past step
			double stepFitness = this->calculateStepFitness();
			
			if(fitnessCalculation == AVERAGE){
				// update the average fitness of the candidate so far
				candidateFitness = (candidateFitness * stepsDone + stepFitness) / (stepsDone + 1);
			}else if (fitnessCalculation == CUMULATIVE){
				candidateFitness += stepFitness;
			}
			
			// mark the past step as completed
			stepsDone++;
		}
		
		/* Check if a running evaluation is now completed */
		if (stepsDone == stepsPerCandidate) {
			// running evaluation has completed. Tell the subclass about this.
			this->setCandidateFitness(candidate, candidateFitness);
			
			// signal that a new candidate must be found
			stepsDone = -1;
			
			// make sure that a 'free' phase will be performed before the new evaluation
			phase = FREEING_RANDOM;
		}
		
		/* Find a new candidate, if required */
		if (stepsDone == -1) { // indicates a new candidate must be found
			
			// ask the subclass for a new candidate
			candidate = this->getNextCandidate();
			candidate->setWorldModel(_wm);

			// reset the counters
			this->stepsDone = 0;
			this->candidateFitness = 0;

			// if the subclass returns NULL, go into the freeing phase again instead of evaluating
			if (candidate == NULL) {
				phase = FREEING_RANDOM;
			}
		}else{
			// let the controller do it's thing
			double left = 0, right = 0;
			candidate->step(left,right);
			this->setSpeed(left,right);
		}
	}
	
	/* Handle the phase in which the robot should move randomly */
	if (phase == FREEING_RANDOM) {
		if (stepsDone == 0) {
			if (debug) { cout << "Starting random free..." << endl; }
			double left = (Rand::randouble() * 2) - 1;
			double right = (Rand::randouble() * 2) - 1;
			this->setSpeed(left, right);
		}
		
		if (stepsDone == stepsPerRandomFree) {
			if (debug) { cout << "... random free done." << endl; }
			phase = FREEING_MANUAL;
			stepsDone = 0;
		} else {
			stepsDone++;
		}
	}
	
	/* Handle the phase in which the free-function should run (in between evaluations) */
	if (phase == FREEING_MANUAL) {
		//if (debug) { cout << "hit..." << endl; }
		if (stepsDone == 0) {
			if (debug) { cout << "Starting manual free..." << endl; }
		}
		
		/* See if the free-controller has run for long enough */
		if (stepsDone == stepsPerFree) {
			// controller is done
			phase = EVALUATING;
			stepsDone = 0;
			if (candidate == NULL) {
				this->stepsDone = -1; // there's no controller for evaluation, find one
			}
			
			if (debug) { cout << "... manual free done." << endl; }
			
		} else {
			/* goal: move the robot as far away from any walls as possible,
			 * or at least out of sensor range */

			// a basic obstacle avoidance behavior
            WallAvoidanceBehavior::step(_wm);
			stepsDone++;
		}
	}
}

ControllerPtr EvolutionaryControlArchitecture::getActiveController(){
	return candidate;
}

double EvolutionaryControlArchitecture::calculateStepFitness() {
	
	// determine the distance traveled, as a fraction of the maximum
	double travelDistance = abs(_wm->_actualTranslationalValue) / abs(_wm->_maxTranslationalDeltaValue);
	
	// determine the difference in bearing (0 = no change, 1 = complete 360 made)
	double bearingDiff = abs(_wm->_actualRotationalVelocity / 360.0);
	
	double fitness = (travelDistance * (1 - bearingDiff));
	//cout << travelDistance << " * (1 - " << bearingDiff << ") = " << fitness << endl;
	return fitness;
}

void EvolutionaryControlArchitecture::reset(){
	candidate.reset();
}
