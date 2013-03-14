/*
 *  MedeaSpecializeSharedData.cpp
 *  Roborobo
 *
 *  Created by Nicolas on 15/09/10.
 *
 */


#include "mEDEA-specialize/include/MedeaSpecializeSharedData.h"


int MedeaSpecializeSharedData::gActiveSun = 1;
double MedeaSpecializeSharedData::gSunLifetime = 0.3; // sun lifetime in generation (0.5 means changes twice per generation, 2 means change every 2 generations

// ----- ----- ----- ----- -----



bool MedeaSpecializeSharedData::gSwarmOnlineObsUsed = false; // define if the SwarmOnlineObserver is used. If it's the case, the following parameter have to be defined and gEnergyMode should be true
bool MedeaSpecializeSharedData::gDynamicSigma = false;
double MedeaSpecializeSharedData::gSigmaMin = 0.0;
double MedeaSpecializeSharedData::gProbSub = 0.0;
double MedeaSpecializeSharedData::gProbAdd = 0.0;
double MedeaSpecializeSharedData::gDynaStep = 0.0;
double MedeaSpecializeSharedData::gSigmaRef = 0.0; // reference value of sigma
double MedeaSpecializeSharedData::gSigmaMax = 0.0; // maximal value of sigma
double MedeaSpecializeSharedData::gProbRef = 0.0; // probability of transmitting the current genome mutated with sigma ref
double MedeaSpecializeSharedData::gProbMax = 0.0; // probability of transmitting the current genome mutatued withe sigma ref
int MedeaSpecializeSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

//int MedeaSpecializeSharedData::gMaxEvaluation = 0; // Roughly how many controllers will be evaluated on a robot. Since there is some restart procedure because of the energy, it might happen that more evaluation take place.

double MedeaSpecializeSharedData::gDriftEvaluationRate = 0.0;
double MedeaSpecializeSharedData::gInitLock = 0.0;
double MedeaSpecializeSharedData::gDriftLock = 0.0;
double MedeaSpecializeSharedData::gMaxKeyRange = 0.0;
double MedeaSpecializeSharedData::gDeltaKey = 0.0;
bool MedeaSpecializeSharedData::gSynchronization = true;
// VALUE DEFINED IN CONSTRUCTOR (below)

int MedeaSpecializeSharedData::gExperimentNumber = 0;
int MedeaSpecializeSharedData::gExperiment1_genStart = 10;
int MedeaSpecializeSharedData::gExperiment2_genStart = 20;

int MedeaSpecializeSharedData::g_xStart_EnergyZone = 0;
int MedeaSpecializeSharedData::g_xEnd_EnergyZone = 0;
int MedeaSpecializeSharedData::g_yStart_EnergyZone = 0;
int MedeaSpecializeSharedData::g_yEnd_EnergyZone = 0;

double MedeaSpecializeSharedData::gZoneEnergy_harvestValue = 0; // set in the code, depends on the following params
double MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue;
double MedeaSpecializeSharedData::gZoneEnergy_minHarvestValue;
int MedeaSpecializeSharedData::gZoneEnergy_maxFullCapacity;
int MedeaSpecializeSharedData::gZoneEnergy_saturateCapacityLevel;

double MedeaSpecializeSharedData::gEnergyMax;
double MedeaSpecializeSharedData::gEnergyRevive;
double MedeaSpecializeSharedData::gEnergyInit;

bool MedeaSpecializeSharedData::gDynamicRespawn;


double MedeaSpecializeSharedData::gMaxPenalizationRate;

// ----- ----- ----- ----- ----- 

bool MedeaSpecializeSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

// ----- ----- ----- ----- ----- 
