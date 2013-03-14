/*
 * MultiKultiInstance.h
 *
 *  Created on: 10/10/2011
 *      Author: fergunet
 */

#ifndef MULTIKULTIINSTANCE_H_
#define MULTIKULTIINSTANCE_H_

#include "MultiKultiController.h"
#include "newscast/HiveMindInstance.h"

class MultiKultiInstance : public HiveMindInstance
{
public:
	MultiKultiInstance();
    ~MultiKultiInstance();

	MultiKultiControllerPtr getDifferentMigrant(int nodeId, MultiKultiControllerPtr message, int eliteSize);
	MultiKultiControllerPtr getRandomMigrant(int nodeId);
	MultiKultiControllerPtr getBestMigrant(int nodeId);
private:
	MultiKultiControllerPtr getDifferentMigrantFromVector(MultiKultiControllerPtr message, vector<MultiKultiControllerPtr> migrants);

	double calculateDifference(MultiKultiControllerPtr a, MultiKultiControllerPtr b);
	//bool comparator(Controller * a, Controller * b);
};



#endif /* MULTIKULTIINSTANCE_H_ */
