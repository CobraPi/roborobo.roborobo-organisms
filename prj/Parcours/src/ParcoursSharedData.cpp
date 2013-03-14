
#include "RoboroboMain/roborobo.h"
#include "../include/ParcoursSharedData.h"

std::string ParcoursSharedData::gGenomesLogFilename;
std::string ParcoursSharedData::gOutputLogFilename;
std::string ParcoursSharedData::gLocationLogFilename;
std::ofstream ParcoursSharedData::outputLogFile;
std::ofstream ParcoursSharedData::locationLogFile;

const int ParcoursSharedData::ZONE_VALUE = 102;
