#include "World/PuckGen.h"
#include "World/Puck.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxPrimitives.h"

#include "World/World.h"

#include "Utilities/Misc.h"

int PuckGen::_nextId = 0;

/* Marsaglia Polar Method. Modification of Misc.cpp::getGaussianRand(double m, double s). */
void getGaussian2D(double xMean, double yMean, double xSigma, double ySigma, Sint16* x, Sint16* y) {
	double u, v, w;	
    do {
		u = 2.0 * ranf() - 1.0;
		v = 2.0 * ranf() - 1.0;
		w = u * u + v * v;
    } while ((w >= 1.0) || (w == 0.0));
    w = sqrt((-2.0 * log(w)) / w);
    *x = (Sint16)(xMean + u*w*xSigma + 0.5);
    *y = (Sint16)(yMean + v*w*ySigma + 0.5);
}



PuckGen::PuckGen(double xMean, double yMean, double xSigma, double ySigma, Uint32 color) {
    _cropX0 = 10;
    _cropY0 = 10;
    _cropX1 = gEnvironmentImage->w - 10;
    _cropY1 = gEnvironmentImage->h - 10;
    _id = _nextId++;
    _xMean = xMean;
    _yMean = yMean;
    _xSigma = xSigma;
    _ySigma = ySigma;
    _puckColor = color;
    _puckEnergy = 1.0;
    gProperties.checkAndGetPropertyValue("puckEnergy", &_puckEnergy, false);
    double radius = 3.0;
    gProperties.checkAndGetPropertyValue("puckRadius", &radius, false);
    _puckRadius = (Sint16)(radius + 0.5);
    _puckMaxAge = -1;
    gProperties.checkAndGetPropertyValue("puckMaxAge", &_puckMaxAge, false);
}


PuckGen::~PuckGen() {
}


inline void PuckGen::generateRandomCoords(Sint16* x, Sint16* y) {
    do {
        getGaussian2D(_xMean, _yMean, _xSigma, _ySigma, x, y);
    } while ((*x < _cropX0) || (*y < _cropY0) || (*x > _cropX1) || (*y > _cropY1));
}


void PuckGen::replace(std::vector< std::vector<int> > *gPuckMap, Puck* puck) {
	replace(gPuckMap, puck, false);
}

void PuckGen::replace(std::vector< std::vector<int> > *gPuckMap, Puck* puck, bool logging) {
	Sint16 xInt, yInt;
	Uint8 colorId;
    if (puck->_isInitialized) {
    	puck->getCoords(xInt, yInt);
    	puck->getColorId(colorId);
    	gLogFile << "PuckTaken: (" << gWorld->getIterations() << ";" << (int)colorId << ";" <<  xInt << ";" << yInt << ")" << std::endl;
        puck->take(gPuckMap);
    } else {
        puck->_isInitialized = true;
        puck->setColor(_id, _puckColor);
    }
    bool locationFound = false;
    for (int tries = 0; tries < MAX_REPLACEMENT_TRIES; tries++) {
        generateRandomCoords(&xInt, &yInt);
        puck->setCoords(xInt, yInt);
        if (puck->testEnvironment()) {locationFound = true; break;}
    }
    if (!locationFound) {
        /*Something bad happens there, cause something is wrong with distribution settings.*/
    }
    puck->setEnergy(_puckEnergy);
    puck->drop(gPuckMap);
}

int PuckGen::getMaxAge() {
    return _puckMaxAge;
}


Sint16 PuckGen::getRadius() {
    return _puckRadius;
}

