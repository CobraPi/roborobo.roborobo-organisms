
#include "RoboroboMain/roborobo.h"
#include "../include/ScalePowerSharedData.h"  

int ScalePowerSharedData::SCALING = LINEAR_SCALING; // default = linear scaling
int ScalePowerSharedData::MAX_ENERGY = 200;
int ScalePowerSharedData::START_ENERGY = 100;

int ScalePowerSharedData::gActiveSun = 1;
double ScalePowerSharedData::gSunLifetime = 0.3; // sun lifetime in generation (0.5 means changes twice per generation, 2 means change every 2 generations

std::string ScalePowerSharedData::gGenomesLogFilename;
std::string ScalePowerSharedData::gOutputLogFilename;
std::ofstream ScalePowerSharedData::genomeLogFile;
std::ofstream ScalePowerSharedData::outputLogFile;