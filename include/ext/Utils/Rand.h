/*
 *  Rand.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RAND_H_
#define RAND_H_

#include <stdlib.h>
#include <cmath>

class Rand {
	
public:
	static int randint(int min, int max);
	static double randouble();
	static double gaussianf(double mu=0.0, double sigma=1.0);
	
};

#endif /* RAND_H_ */