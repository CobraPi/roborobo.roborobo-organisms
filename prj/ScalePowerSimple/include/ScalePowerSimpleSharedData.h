/* 
 * File:   ScalePowerSimpleSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef ScalePowerSimpleSHAREDDATA_H
#define	ScalePowerSimpleSHAREDDATA_H

class ScalePowerSimpleSharedData {
public:
	static const int LOGARITHMIC_SCALING = 1;
	static const int LINEAR_SCALING = 2;
	static const int EXPONENTIAL_SCALING = 3;
	
	static int gActiveSun; // current active sun (index of energy point)
	static double gSunLifetime; // sun lifetime in generation (0.5 means changes twice per generation, 2 means change every 2 generations)

	static int SCALING;
	static int MAX_ENERGY;
	static int START_ENERGY;
	
	static std::string gGenomesLogFilename;
	static std::string gOutputLogFilename;
	static std::ofstream genomeLogFile;
	static std::ofstream outputLogFile;
private:

};


#endif	/* ScalePowerSimpleSHAREDDATA_H */

