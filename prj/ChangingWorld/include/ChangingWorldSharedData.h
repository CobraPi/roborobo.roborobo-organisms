/* 
 * File:   ChangingWorldSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef ChangingWorldSHAREDDATA_H
#define	ChangingWorldSHAREDDATA_H

class ChangingWorldSharedData {
public:
	static std::string gOutputLogFilename;
	static std::string gLocationLogFilename;
	static std::ofstream outputLogFile;
	static std::ofstream locationLogFile;
	
	static const int ZONE_VALUE;
    
    static std::vector<int> scaleFactor;
    static int currentFactor;
    static int factorLifetime;
	static int maxEnergy;
	static int startEnergy;
private:

};


#endif	/* ChangingWorldSHAREDDATA_H */

