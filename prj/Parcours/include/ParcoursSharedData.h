/* 
 * File:   ParcoursSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef ParcoursSHAREDDATA_H
#define	ParcoursSHAREDDATA_H

class ParcoursSharedData {
public:
	static std::string gGenomesLogFilename;
	static std::string gOutputLogFilename;
	static std::string gLocationLogFilename;
	static std::ofstream outputLogFile;
	static std::ofstream locationLogFile;
	
	static const int ZONE_VALUE;
private:

};


#endif	/* ParcoursSHAREDDATA_H */

