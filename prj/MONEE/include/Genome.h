/*
 * Genome.h
 *
 *  Created on: Mar 3, 2013
 *      Author: job
 */
#include "RoboroboMain/common.h"

#ifndef GENOME_H_
#define GENOME_H_

class Genome {
public:
	Genome();
	virtual ~Genome();

	int id;
	std::vector<double> parameters;
	std::vector<int> pucks;
	std::vector<double> sms;	// Sensori-Motor-Stream data
	double fitness;
};

#endif /* GENOME_H_ */
