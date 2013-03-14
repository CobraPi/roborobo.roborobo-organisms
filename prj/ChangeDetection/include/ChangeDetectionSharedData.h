/* 
 * File:   ChangeDetectionSharedData.h
 * Author: bweel
 *
 * Created on April 26, 2011, 12:31 PM
 */

#ifndef ChangeDetectionSHAREDDATA_H
#define	ChangeDetectionSHAREDDATA_H

class ChangeDetectionSharedData {
public:
	static std::string gOutputLogFilename;
	static std::string gLocationLogFilename;
	static std::ofstream outputLogFile;
	static std::ofstream locationLogFile;
	
	static const int ZONE_VALUE;
private:

};


#endif	/* ChangeDetectionSHAREDDATA_H */

