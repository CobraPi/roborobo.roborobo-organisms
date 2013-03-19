#ifndef MONEECONTROLARCHITECTURE_H
#define MONEECONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "MONEE/include/MONEEAgentWorldModel.h"

#include "World/World.h"
#include "Agents/RobotAgent.h"

#include "Utilities/Misc.h"
#include "Utils/Rand.h"

#include "Sensor/PuckSensors.h"

#include "MONEEWorldObserver.h"

#include <cmath>

class MONEEControlArchitecture : public BehaviorControlArchitecture {
        MONEEAgentWorldModel* _wm;

        int _maxLifetime[2];

        int _hiddenNeuronCount;
        int _parameterCount;
        
        bool _useMarket;
        bool _useSpecBonus;
        bool _randomSelection;
        double _task1Premium;
        double _selectionPressure;

        std::vector<double> _response;

        void assignFitness(std::vector<Genome> & genomes);
        void gatherGenomes(std::vector<Genome*> & genePool, int commDistSquared);

        std::vector<Genome*> _nearbyGenomes;

        Genome _activeGenome;

        std::vector<Genome>::const_iterator selectWeighted(std::vector<Genome> & genomes);

        std::vector<PuckSensors*> _rangeSensors;

	public:
		MONEEControlArchitecture(RobotAgentWorldModel *__wm);
		~MONEEControlArchitecture();

		void step();

        void reset();
		void updateActuators();
		void updateGenomes();
        void select();

		Genome* getGenome() {return &_activeGenome;}
};


#endif

