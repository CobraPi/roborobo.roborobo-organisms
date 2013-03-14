
#include "ChangeDetection/include/ChangeDetectionAgentWorldModel.h"

#include "World/World.h"

ChangeDetectionAgentWorldModel::ChangeDetectionAgentWorldModel() {
	//_sensors = NULL;
	//_ditchSensor = NULL;
	swarmTime = 0;
	organismTime = 0;
	penalty = 0;
	ditchCheck = false;
    xStart = -1;
}

ChangeDetectionAgentWorldModel::~ChangeDetectionAgentWorldModel() {
}
