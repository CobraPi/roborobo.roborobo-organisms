/*
 *  MedeaSharedData.cpp
 *  Roborobo
 *
 *  Created by Nicolas on 3/6/2010.
 *
 */


#include "mEDEA/include/MedeaSharedData.h"



// ----- ----- ----- ----- -----



bool MedeaSharedData::gSwarmOnlineObsUsed = false; // define if the SwarmOnlineObserver is used. If it's the case, the following parameter have to be defined and gEnergyMode should be true
bool MedeaSharedData::gDynamicSigma = false;
double MedeaSharedData::gSigmaMin = 0.0;
double MedeaSharedData::gProbSub = 0.0;
double MedeaSharedData::gProbAdd = 0.0;
double MedeaSharedData::gDynaStep = 0.0;
double MedeaSharedData::gSigmaRef = 0.0; // reference value of sigma
double MedeaSharedData::gSigmaMax = 0.0; // maximal value of sigma
double MedeaSharedData::gProbRef = 0.0; // probability of transmitting the current genome mutated with sigma ref
double MedeaSharedData::gProbMax = 0.0; // probability of transmitting the current genome mutatued withe sigma ref
int MedeaSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

//int MedeaSharedData::gMaxEvaluation = 0; // Roughly how many controllers will be evaluated on a robot. Since there is some restart procedure because of the energy, it might happen that more evaluation take place.

double MedeaSharedData::gDriftEvaluationRate = 0.0;
double MedeaSharedData::gInitLock = 0.0;
double MedeaSharedData::gDriftLock = 0.0;
double MedeaSharedData::gMaxKeyRange = 0.0;
double MedeaSharedData::gDeltaKey = 0.0;
bool MedeaSharedData::gSynchronization = true;
// VALUE DEFINED IN CONSTRUCTOR (below)

int MedeaSharedData::gExperimentNumber = 0;
int MedeaSharedData::gExperiment1_genStart = 10;
int MedeaSharedData::gExperiment2_genStart = 20;

int MedeaSharedData::g_xStart_EnergyZone = 0;
int MedeaSharedData::g_xEnd_EnergyZone = 0;
int MedeaSharedData::g_yStart_EnergyZone = 0;
int MedeaSharedData::g_yEnd_EnergyZone = 0;

double MedeaSharedData::gZoneEnergy_harvestValue = 0; // set in the code, depends on the following params
double MedeaSharedData::gZoneEnergy_maxHarvestValue;
double MedeaSharedData::gZoneEnergy_minHarvestValue;
int MedeaSharedData::gZoneEnergy_maxFullCapacity;
int MedeaSharedData::gZoneEnergy_saturateCapacityLevel;

double MedeaSharedData::gEnergyMax;
double MedeaSharedData::gEnergyRevive;
double MedeaSharedData::gEnergyInit;

bool MedeaSharedData::gDynamicRespawn;


double MedeaSharedData::gMaxPenalizationRate;

// ----- ----- ----- ----- ----- 

bool MedeaSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

// ----- ----- ----- ----- ----- 
