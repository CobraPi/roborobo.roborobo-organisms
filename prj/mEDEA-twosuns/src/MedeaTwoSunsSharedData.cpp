/*
 *  MedeaTwoSunsSharedData.cpp
 *  Roborobo
 *
 *  Created by Nicolas on 3/6/2010.
 *
 */


#include "mEDEA-twosuns/include/MedeaTwoSunsSharedData.h"


int MedeaTwoSunsSharedData::gActiveSun = 1;
double MedeaTwoSunsSharedData::gSunLifetime = 0.3; // sun lifetime in generation (0.5 means changes twice per generation, 2 means change every 2 generations

// ----- ----- ----- ----- -----



bool MedeaTwoSunsSharedData::gSwarmOnlineObsUsed = false; // define if the SwarmOnlineObserver is used. If it's the case, the following parameter have to be defined and gEnergyMode should be true
bool MedeaTwoSunsSharedData::gDynamicSigma = false;
double MedeaTwoSunsSharedData::gSigmaMin = 0.0;
double MedeaTwoSunsSharedData::gProbSub = 0.0;
double MedeaTwoSunsSharedData::gProbAdd = 0.0;
double MedeaTwoSunsSharedData::gDynaStep = 0.0;
double MedeaTwoSunsSharedData::gSigmaRef = 0.0; // reference value of sigma
double MedeaTwoSunsSharedData::gSigmaMax = 0.0; // maximal value of sigma
double MedeaTwoSunsSharedData::gProbRef = 0.0; // probability of transmitting the current genome mutated with sigma ref
double MedeaTwoSunsSharedData::gProbMax = 0.0; // probability of transmitting the current genome mutatued withe sigma ref
int MedeaTwoSunsSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

//int MedeaTwoSunsSharedData::gMaxEvaluation = 0; // Roughly how many controllers will be evaluated on a robot. Since there is some restart procedure because of the energy, it might happen that more evaluation take place.

double MedeaTwoSunsSharedData::gDriftEvaluationRate = 0.0;
double MedeaTwoSunsSharedData::gInitLock = 0.0;
double MedeaTwoSunsSharedData::gDriftLock = 0.0;
double MedeaTwoSunsSharedData::gMaxKeyRange = 0.0;
double MedeaTwoSunsSharedData::gDeltaKey = 0.0;
bool MedeaTwoSunsSharedData::gSynchronization = true;
// VALUE DEFINED IN CONSTRUCTOR (below)

int MedeaTwoSunsSharedData::gExperimentNumber = 0;
int MedeaTwoSunsSharedData::gExperiment1_genStart = 10;
int MedeaTwoSunsSharedData::gExperiment2_genStart = 20;

int MedeaTwoSunsSharedData::g_xStart_EnergyZone = 0;
int MedeaTwoSunsSharedData::g_xEnd_EnergyZone = 0;
int MedeaTwoSunsSharedData::g_yStart_EnergyZone = 0;
int MedeaTwoSunsSharedData::g_yEnd_EnergyZone = 0;

double MedeaTwoSunsSharedData::gZoneEnergy_harvestValue = 0; // set in the code, depends on the following params
double MedeaTwoSunsSharedData::gZoneEnergy_maxHarvestValue;
double MedeaTwoSunsSharedData::gZoneEnergy_minHarvestValue;
int MedeaTwoSunsSharedData::gZoneEnergy_maxFullCapacity;
int MedeaTwoSunsSharedData::gZoneEnergy_saturateCapacityLevel;

double MedeaTwoSunsSharedData::gEnergyMax;
double MedeaTwoSunsSharedData::gEnergyRevive;
double MedeaTwoSunsSharedData::gEnergyInit;

bool MedeaTwoSunsSharedData::gDynamicRespawn;


double MedeaTwoSunsSharedData::gMaxPenalizationRate;

// ----- ----- ----- ----- ----- 

bool MedeaTwoSunsSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

// ----- ----- ----- ----- ----- 
