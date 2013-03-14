
#include <math.h>

#include "Parcours/include/ParcoursWorldObserver.h"
#include "Parcours/include/ParcoursSharedData.h"

ParcoursWorldObserver::ParcoursWorldObserver(World *__world) : WorldObserver(__world) {
	_world = __world;
}

ParcoursWorldObserver::~ParcoursWorldObserver() {
	// nothing to do.
}

void ParcoursWorldObserver::showOrganisms() {
}

void ParcoursWorldObserver::reset() {
	if(ParcoursSharedData::outputLogFile){
		ParcoursSharedData::outputLogFile.close();
	}
	ParcoursSharedData::outputLogFile.open(ParcoursSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ParcoursSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << ParcoursSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ParcoursSharedData::outputLogFile << "id,timestep,outputs,choice,desired organism size,direction,speed" << std::endl;
	
	
	if(ParcoursSharedData::locationLogFile){
		ParcoursSharedData::locationLogFile.close();
	}
	ParcoursSharedData::locationLogFile.open(ParcoursSharedData::gLocationLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!ParcoursSharedData::locationLogFile) {
		std::cout << "[error] Cannot open log file" << ParcoursSharedData::gLocationLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	ParcoursSharedData::locationLogFile << "id,timestep,x,y,in org,org_size,swarm time,organism time" << std::endl;
}

void ParcoursWorldObserver::step() {

}
