/*
 *  Rand.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utils/Rand.h"

/* Get a random number in the range [min,max) */
int Rand::randint(int minInclusive, int maxExclusive) {
    //return (int)((double)randouble()/(double)(maxExclusive-minInclusive)+minInclusive);
    return (rand() % (maxExclusive - minInclusive)) + minInclusive;
}

double Rand::randouble() {
    int random = RAND_MAX;
    while (random == RAND_MAX) random = rand();
    return (double) random / (double) (RAND_MAX);
}

double Rand::gaussianf(double mu /* = 0.0 */, double sigma /* = 1.0 */) {
    static bool deviateAvailable = false;
    static double storedDeviate;
    double polar, w, y1, y2;
    if (!deviateAvailable) {
        do {
            y1 = 2.0 * randouble() - 1.0;
            y2 = 2.0 * randouble() - 1.0;
            w = y1 * y1 + y2 * y2;
        } while (w >= 1.0);
        polar = sqrt((-2.0 * log(w)) / w);
        deviateAvailable = true;
        storedDeviate = y2 * polar;
        return y1 * polar * sigma + mu;
    } else {
        deviateAvailable = false;
        return storedDeviate * sigma + mu;
    }
}
