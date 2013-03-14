#ifndef PUCKGEN_H
#define PUCKGEN_H

#include "RoboroboMain/common.h"
#include "Utilities/Misc.h"

class Puck;

/* Gaussian distribution of single color. */
// TODO: Other types of distributions.

static const int MAX_REPLACEMENT_TRIES = 100;

void getGaussian2D(double mean, double sigma, Sint16* x, Sint16* y);

class PuckGen {
    private :
        static int _nextId; int _id;
        double _xMean, _yMean, _xSigma, _ySigma;
        Uint32 _puckColor;
        Sint16 _puckRadius;
        double _puckEnergy;
        int _puckMaxAge;
        Sint16 _cropX0, _cropY0, _cropX1, _cropY1;
        inline void generateRandomCoords(Sint16* x, Sint16* y);
    public :
        PuckGen(double xMean, double yMean, double xSigma, double ySigma, Uint32 color);
        ~PuckGen();
        void replace(std::vector< std::vector<int> > *gPuckMap, Puck* puck);
        void replace(std::vector< std::vector<int> > *gPuckMap, Puck* puck, bool logging);
        int getMaxAge();
        Sint16 getRadius();
        
        int getId() {return _id;}
};

#endif 
