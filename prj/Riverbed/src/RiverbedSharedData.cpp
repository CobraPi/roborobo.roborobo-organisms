
#include "RoboroboMain/roborobo.h"
#include "../include/RiverbedSharedData.h"  

int RiverbedSharedData::SCALING = LINEAR_SCALING; // default = linear scaling
int RiverbedSharedData::MAX_ENERGY = 200;
int RiverbedSharedData::START_ENERGY = 100;

std::string RiverbedSharedData::gGenomesLogFilename;
std::string RiverbedSharedData::gOutputLogFilename;
std::ofstream RiverbedSharedData::genomeLogFile;
std::ofstream RiverbedSharedData::outputLogFile;

double RiverbedSharedData::CURRENT_STRENGTH = 2;
int RiverbedSharedData::CURRENT_DIRECTION = 90;
