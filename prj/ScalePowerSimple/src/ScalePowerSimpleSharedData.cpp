
#include "RoboroboMain/roborobo.h"
#include "../include/ScalePowerSimpleSharedData.h"  

int ScalePowerSimpleSharedData::SCALING = LINEAR_SCALING; // default = linear scaling
int ScalePowerSimpleSharedData::MAX_ENERGY = 200;
int ScalePowerSimpleSharedData::START_ENERGY = 100;

int ScalePowerSimpleSharedData::gActiveSun = 1;
double ScalePowerSimpleSharedData::gSunLifetime = 0.3; // sun lifetime in generation (0.5 means changes twice per generation, 2 means change every 2 generations

std::string ScalePowerSimpleSharedData::gGenomesLogFilename;
std::string ScalePowerSimpleSharedData::gOutputLogFilename;
std::ofstream ScalePowerSimpleSharedData::genomeLogFile;
std::ofstream ScalePowerSimpleSharedData::outputLogFile;