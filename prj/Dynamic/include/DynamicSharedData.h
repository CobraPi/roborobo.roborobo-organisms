/* 
 * File:   DynamicSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef DYNAMICSHAREDDATA_H
#define	DYNAMICSHAREDDATA_H

class DynamicSharedData {
public:
	static const int LOGARITHMIC_SCALING = 1;
	static const int LINEAR_SCALING = 2;
	static const int EXPONENTIAL_SCALING = 3;

	static int SCALING;
	static int MAX_ENERGY;
	static int START_ENERGY;
    
    static int NUM_PLANTS;
    static int NUM_WEIGHTS;
    static double slope;
    static double slopeChange;
    static bool reverse;
	
	static std::string gGenomesLogFilename;
	static std::string gOutputLogFilename;
    static std::string gLocationLogFilename;
    
	static std::ofstream genomeLogFile;
	static std::ofstream outputLogFile;
    static std::ofstream locationLogFile;

	static int gEnvironmentLifetime;	// Jan-Willem: Change environment after X generations
    
};


#endif	/* SCALEPOWERSHAREDDATA_H */

