
#include "RoboroboMain/roborobo.h"
#include "../include/DynamicSharedData.h"

int DynamicSharedData::SCALING = LINEAR_SCALING; // default = linear scaling
int DynamicSharedData::MAX_ENERGY = 200;
int DynamicSharedData::START_ENERGY = 100;

int DynamicSharedData::NUM_PLANTS = 10;
int DynamicSharedData::NUM_WEIGHTS = 1;
double DynamicSharedData::slope = -1;
double DynamicSharedData::slopeChange = -0.5;
bool DynamicSharedData::reverse = true;

std::string DynamicSharedData::gGenomesLogFilename;
std::string DynamicSharedData::gOutputLogFilename;
std::string DynamicSharedData::gLocationLogFilename;

std::ofstream DynamicSharedData::genomeLogFile;
std::ofstream DynamicSharedData::outputLogFile;
std::ofstream DynamicSharedData::locationLogFile;

int DynamicSharedData::gEnvironmentLifetime = 125;