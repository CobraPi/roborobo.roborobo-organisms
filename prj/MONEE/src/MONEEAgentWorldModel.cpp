#include "MONEE/include/MONEEAgentWorldModel.h"
#include "World/World.h"
#include <numeric>


MONEEAgentWorldModel::MONEEAgentWorldModel() {
    setRobotLED_status(true);
    _action = ACTION_ACTIVATE;
    _speedPenalty = 1.0;
    _specialisation = false;

    gProperties.checkAndGetPropertyValue("gSpecialisation", &_specialisation, false);
    //std::cout << "Specialisation: " << _specialisation << '\n';
}

MONEEAgentWorldModel::~MONEEAgentWorldModel() {
}

void MONEEAgentWorldModel::dumpGenePoolStats() const {
    std::vector<Genome>::const_reverse_iterator itGenome;
    double totalFitness = .0;
    gLogFile << "EggHatched: (" << gWorld->getIterations() << "; " << _xReal << "; " << _yReal << "); [";   
    for (itGenome = _genePool.rbegin(); itGenome < _genePool.rend(); itGenome++) {
        if (itGenome != _genePool.rbegin()) {
            gLogFile << ", ";
        }
        gLogFile << "[";
        for (int i = 0; i < gPuckColors; i++) {
            gLogFile << itGenome->pucks[i] << ", ";
        }
        gLogFile << itGenome->fitness << ", " << itGenome->id << "]";
        totalFitness += itGenome->fitness;
    }
    gLogFile << "]; Winner: " << _winnerId << std::endl;
}

void MONEEAgentWorldModel::setSpeedPenalty() {
    //std::cout << "Specialisation: " << _specialisation << '\n';
    if (!_specialisation) 
    {
        _speedPenalty = 1.0;
    } 
    else if (_puckCounters && ! _puckCounters->empty())
    {
        double max = (double) *(std::max_element(_puckCounters->begin(), _puckCounters->end()));
        double sum = (double) std::accumulate(_puckCounters->begin(), _puckCounters->end(), 0);
	double x = (sum == 0.0) ? 1.0 : (max/sum); // x is the specialisation level 

        _speedPenalty = x;
    }
    //std::cout << "Speed penalty: " << _speedPenalty << '\n';
}
