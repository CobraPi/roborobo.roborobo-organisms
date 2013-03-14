#ifndef SYMBRIONSHAREDDATA_H
#define SYMBRIONSHAREDDATA_H

#include <string>
#include <fstream>

class SymbrionSharedData {
public:
	static std::string gGenomesOrgLogFilename;
	static std::string gOutputLogFilename;
	static std::string gEventLogFilename;
	static std::string gTravelLogFilename;
	static std::string gHeatmapLogFilename;
	static std::string gPopLogFilename;
	static std::ofstream genomeLogFile;
	static std::ofstream outputLogFile;
	static std::ofstream gGenomesOrgLogFile;
	static std::ofstream gEventLogFile;
	static std::ofstream gTravelLogFile;
	static std::ofstream gPopLogFile;
    
    static std::string gEggAgentMaskImageFilename;
    static std::string gSeedAgentMaskImageFilename;
    static std::string gFreeRiderAgentMaskImageFilename;
    
    static std::string gEggAgentSpecsImageFilename;
    static std::string gSeedAgentSpecsImageFilename;
    static std::string gFreeRiderAgentSpecsImageFilename;
    
    static SDL_Surface *gEggAgentMaskImage;
    static SDL_Surface *gEggAgentSpecsImage;
    static SDL_Surface *gSeedAgentMaskImage;
    static SDL_Surface *gSeedAgentSpecsImage;
    static SDL_Surface *gFreeRiderAgentMaskImage;
    static SDL_Surface *gFreeRiderAgentSpecsImage;
	
	static int maxEggLifetime;
	static int maxSeedLifetime;
	static int maxOrganismLifetime;
	
	static int maxSeedBoardcastRange;
	static int maxOrganismBoardcastRange;
	
	static int maxOrganismSize;
	static int minOrganismSize;
    
    static unsigned int nrAbortions;
	
	// 0 = no connections
	// 1 = input to input
	// 2 = input to hidden
	// 3 = input to output
	// 4 = hidden to input
	// 5 = hidden to hidden
	// 6 = hidden to output
	// 7 = output to input 
	// 8 = output to hidden
	// 9 = output to output
	static int connectionType;
private:
};

enum AgentState { EGG, SEED, ADULT, FREERIDER };

#endif

