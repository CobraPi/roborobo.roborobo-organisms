#include "RoboroboMain/roborobo.h"
#include "Symbrion/include/SymbrionSharedData.h"

std::string SymbrionSharedData::gGenomesOrgLogFilename;
std::string SymbrionSharedData::gOutputLogFilename;
std::string SymbrionSharedData::gEventLogFilename;
std::string SymbrionSharedData::gTravelLogFilename;
std::string SymbrionSharedData::gHeatmapLogFilename;
std::string SymbrionSharedData::gPopLogFilename;
std::ofstream SymbrionSharedData::genomeLogFile;
std::ofstream SymbrionSharedData::gGenomesOrgLogFile;
std::ofstream SymbrionSharedData::outputLogFile;
std::ofstream SymbrionSharedData::gEventLogFile;
std::ofstream SymbrionSharedData::gTravelLogFile;
std::ofstream SymbrionSharedData::gPopLogFile;

int SymbrionSharedData::maxEggLifetime;
int SymbrionSharedData::maxSeedLifetime;
int SymbrionSharedData::maxOrganismLifetime;
int SymbrionSharedData::maxSeedBoardcastRange;
int SymbrionSharedData::maxOrganismBoardcastRange;

int SymbrionSharedData::minOrganismSize;
int SymbrionSharedData::maxOrganismSize;

int SymbrionSharedData::connectionType;

unsigned int SymbrionSharedData::nrAbortions = 0;

std::string SymbrionSharedData::gEggAgentMaskImageFilename = "data/egg-agent-mask.png";
std::string SymbrionSharedData::gSeedAgentMaskImageFilename = "data/seed-agent-mask.png";
std::string SymbrionSharedData::gFreeRiderAgentMaskImageFilename = "data/freerider-agent-mask.png";

std::string SymbrionSharedData::gEggAgentSpecsImageFilename = gAgentSpecsImageFilename;
std::string SymbrionSharedData::gSeedAgentSpecsImageFilename = gAgentSpecsImageFilename;
std::string SymbrionSharedData::gFreeRiderAgentSpecsImageFilename = gAgentSpecsImageFilename;

SDL_Surface *SymbrionSharedData::gEggAgentMaskImage = NULL;
SDL_Surface *SymbrionSharedData::gEggAgentSpecsImage = NULL;
SDL_Surface *SymbrionSharedData::gSeedAgentMaskImage = NULL;
SDL_Surface *SymbrionSharedData::gSeedAgentSpecsImage = NULL;
SDL_Surface *SymbrionSharedData::gFreeRiderAgentMaskImage = NULL;
SDL_Surface *SymbrionSharedData::gFreeRiderAgentSpecsImage = NULL;
