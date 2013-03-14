
#include "RoboroboMain/roborobo.h"
#include "../include/ChangingWorldSharedData.h"

std::string ChangingWorldSharedData::gOutputLogFilename;
std::string ChangingWorldSharedData::gLocationLogFilename;
std::ofstream ChangingWorldSharedData::outputLogFile;
std::ofstream ChangingWorldSharedData::locationLogFile;

const int ChangingWorldSharedData::ZONE_VALUE = 102;

int ChangingWorldSharedData::maxEnergy = 200;
int ChangingWorldSharedData::startEnergy = 100;
std::vector<int> ChangingWorldSharedData::scaleFactor;
int ChangingWorldSharedData::currentFactor = -1;
int ChangingWorldSharedData::factorLifetime = 50;
