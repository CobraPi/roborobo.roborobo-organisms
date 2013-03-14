
#include <math.h>

#include "ChangeDetection/include/ChangeDetectionWorldObserver.h"
#include "ChangeDetection/include/ChangeDetectionSharedData.h"

ChangeDetectionWorldObserver::ChangeDetectionWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

ChangeDetectionWorldObserver::~ChangeDetectionWorldObserver() {
	// nothing to do.
}

void ChangeDetectionWorldObserver::showOrganisms() {
}

void ChangeDetectionWorldObserver::reset() {
	if(ChangeDetectionSharedData::outputLogFile){
		ChangeDetectionSharedData::outputLogFile.close();
	}
	ChangeDetectionSharedData::outputLogFile.open(ChangeDetectionSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ChangeDetectionSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << ChangeDetectionSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ChangeDetectionSharedData::outputLogFile << "id,timestep,outputs,choice,desired organism size,direction,speed" << std::endl;
	
	
	if(ChangeDetectionSharedData::locationLogFile){
		ChangeDetectionSharedData::locationLogFile.close();
	}
	ChangeDetectionSharedData::locationLogFile.open(ChangeDetectionSharedData::gLocationLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ChangeDetectionSharedData::locationLogFile) {
		std::cout << "[error] Cannot open log file" << ChangeDetectionSharedData::gLocationLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ChangeDetectionSharedData::locationLogFile << "id,timestep,x,y,in org,org_size,swarm time,organism time" << std::endl;
}

void ChangeDetectionWorldObserver::step() {

}
