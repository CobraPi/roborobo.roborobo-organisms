#ifndef MONEEWORLDOBSERVER_H
#define MONEEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"
#include "Utilities/Timer.h"

#include "World/Puck.h"
#include "World/PuckGen.h"
#include "Genome.h"

#include <ios>
#include <iostream>

class World;

///from roborobo.h///
extern int gPuckCount;
extern int gPuckColors;
extern std::vector<Puck> gPucks;
extern std::vector<PuckGen> gPuckGens;
extern std::vector< std::vector< int > > gPuckMap;
extern std::vector<Genome> gGenomes;
extern bool gUseDitchSensors;
extern Uint16 gMaxGenePool;
extern Uint32 gNextGenomeId;
extern int gApproximateDiameterSquared;
extern std::vector<int> gDistSquared;
extern int gMaxCommDistSquared;
extern std::vector<bool> gBoolRadioNetworkArray; // a 2D adjacent matrix stating network connections and signal strength. By convention, 0 means no signal.

inline bool isRadioConnection(int & idOne, int & idTwo) {
    int index = (idOne < idTwo) ? idOne * gAgentCounter + idTwo : idTwo * gAgentCounter + idOne;
    return gBoolRadioNetworkArray[index];
}

inline bool isCollisionPotential(int & idOne, int & idTwo) {
	int index = (idOne < idTwo) ? idOne * gAgentCounter + idTwo : idTwo * gAgentCounter + idOne;
    return gDistSquared[index] < gApproximateDiameterSquared * 9;
}
///end from roborobo.h///


class MONEEWorldObserver : public WorldObserver
{
	private:
		Timer *timer;
	protected:

	public:
		Uint32 _colorChangeTarget;
		int _colorChangeIteration;

		MONEEWorldObserver( World *__world );
		~MONEEWorldObserver();

		void reset();
		void step();

        void doColorChange();
};

#endif

