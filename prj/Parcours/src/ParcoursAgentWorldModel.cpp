
#include "Parcours/include/ParcoursAgentWorldModel.h"

#include "World/World.h"

ParcoursAgentWorldModel::ParcoursAgentWorldModel() {
	//_sensors = NULL;
	//_ditchSensor = NULL;
	swarmTime = 0;
	organismTime = 0;
	penalty = 0;
	ditchCheck = false;
    xStart = -1;
}

ParcoursAgentWorldModel::~ParcoursAgentWorldModel() {
}
