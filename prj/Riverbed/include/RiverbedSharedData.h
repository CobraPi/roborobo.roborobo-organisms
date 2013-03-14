/* 
 * File:   RiverbedSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef RIVERBEDSHAREDDATA_H
#define RIVERBEDSHAREDDATA_H

class RiverbedSharedData {
public:   
	static const int LOGARITHMIC_SCALING = 1;
	static const int LINEAR_SCALING = 2;
	static const int EXPONENTIAL_SCALING = 3;
	
	static int SCALING;
	static int MAX_ENERGY;
	static int START_ENERGY;
	
	static double CURRENT_STRENGTH;
	static int CURRENT_DIRECTION;
	
	static std::string gGenomesLogFilename;
	static std::string gOutputLogFilename;
	static std::ofstream genomeLogFile;
	static std::ofstream outputLogFile;
private:
    
};


#endif	/* SCALEPOWERSHAREDDATA_H */

