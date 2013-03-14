
#include "RoboroboMain/roborobo.h"
#include "../include/ChangeDetectionSharedData.h"

std::string ChangeDetectionSharedData::gOutputLogFilename;
std::string ChangeDetectionSharedData::gLocationLogFilename;
std::ofstream ChangeDetectionSharedData::outputLogFile;
std::ofstream ChangeDetectionSharedData::locationLogFile;

const int ChangeDetectionSharedData::ZONE_VALUE = 102;
