/*
 *  MultiKultiController.h
 *  roborobo
 *
 *  Created by B.P.M. Weel on 3/20/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MULTI_KULTI_CONTROLLER_H__
#define __MULTI_KULTI_CONTROLLER_H__

#include "RoboroboMain/roborobo.h"
#include "Utils/Controller.h"

class MultiKultiController;
typedef boost::shared_ptr<MultiKultiController> MultiKultiControllerPtr;

class MultiKultiController : public Controller {
public:
	MultiKultiController() {}
	MultiKultiController(vector<double> _mutationStepSizes) : Controller(_mutationStepSizes) {}
	virtual ~MultiKultiController() {}
	
	virtual double calculateDifference(MultiKultiControllerPtr other) = 0;
	
	bool operator<(const MultiKultiController& other) const
	{
		return this->fitness < other.fitness;
	}
};

#endif // __MULTI_KULTI_CONTROLLER_H__
