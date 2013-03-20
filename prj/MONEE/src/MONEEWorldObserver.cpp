#include "MONEE/include/MONEEWorldObserver.h"

#include "World/World.h"
#include "zsu/Properties.h"
#include "Organism.h"
#include "Config/GlobalConfigurationLoader.h"
#include "Agents/RobotAgent.h"

///from roborobo.cpp///
int gPuckCount;
int gPuckColors;
std::vector<Puck> gPucks;
std::vector<PuckGen> gPuckGens;
std::vector< std::vector< int > > gPuckMap;
std::vector<Genome> gGenomes;

Uint16 gMaxGenePool;
int gMaxCommDistSquared;

Uint32 gNextGenomeId;

bool gUseDitchSensors;

std::vector<bool> gBoolRadioNetworkArray; // initialized in gWorld->initWorld();

int gApproximateDiameterSquared;
std::vector<int> gDistSquared;
///end from roborobo.cpp///

MONEEWorldObserver::MONEEWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;

	///from world.cpp///
	double tmpReal = 0; int tmpInt = 0;
	gProperties.checkAndGetPropertyValue("gMaxCommDist", &tmpReal, true);
	gMaxCommDistSquared = tmpReal * tmpReal;

	gProperties.checkAndGetPropertyValue("gColorChangeTarget", &tmpInt, true);
	_colorChangeTarget = tmpInt;
	gProperties.checkAndGetPropertyValue("gColorChangeIteration", &tmpInt, true);
	_colorChangeIteration = tmpInt;

	if (gEnergyMode) {
		// Should be declared before agent models are created
		gProperties.checkAndGetPropertyValue("gPuckColors", &gPuckColors, true);

		double w = gBackgroundImage->w;
		double h = gBackgroundImage->h;

		gPuckMap.resize(w);
		for (int i = 0; i < w; i++) gPuckMap[i].resize(h);

		double xMean, yMean, xSigma, ySigma;

		std::vector<int> puckCount;
		puckCount.reserve(gPuckColors);
		gPuckGens.reserve(gPuckColors);

		gPuckCount = 0;

		for (int puckGenIdx = 0; puckGenIdx < gPuckColors; puckGenIdx++) {
			std::stringstream ss; ss << "puckGen[" << puckGenIdx << "]";
			std::string base = ss.str();
			gProperties.checkAndGetPropertyValue(base +".xMean", &tmpReal, true);
			xMean = w * tmpReal;
			gProperties.checkAndGetPropertyValue(base + ".yMean", &tmpReal, true);
			yMean = h * tmpReal;
			gProperties.checkAndGetPropertyValue(base + ".xSigma", &tmpReal, true);
			xSigma = w * tmpReal;
			gProperties.checkAndGetPropertyValue(base + ".ySigma", &tmpReal, true);
			ySigma = h * tmpReal;
			PuckGen newPuckGen(xMean, yMean, xSigma, ySigma, Puck::PALETTE[puckGenIdx]);
			gPuckGens.push_back(newPuckGen);

			gProperties.checkAndGetPropertyValue(base + ".count", &tmpInt, true);
			puckCount.push_back(tmpInt);
			gPuckCount += tmpInt;
		}

		gPucks.reserve(gPuckCount);

		for (Uint8 i = 0; i < puckCount.size(); i++) {
			int pucksToGo = puckCount[i];
			while (pucksToGo > 0) {
				Puck newPuck(&gPuckGens[i]);
				gPucks.push_back(newPuck);
				pucksToGo--;
			}
		}
	}

	gUseDitchSensors = false;

	G_COLOR_WHITE = SDL_MapRGB(gForegroundImage->format, 0xFF, 0xFF, 0xFF);
	///end from world.cpp///
}

MONEEWorldObserver::~MONEEWorldObserver()
{
	// nothing to do.
}

void MONEEWorldObserver::doColorChange() {
    for (int i = 0; i < gPuckCount; i++) gPucks[i].recolor(&gPuckMap, _colorChangeTarget, Puck::PALETTE[_colorChangeTarget]);
}

void MONEEWorldObserver::reset()
{
	double tmpReal = 0; int tmpInt = 0;
	gProperties.checkAndGetPropertyValue("gMaxCommDist", &tmpReal, true);
	gMaxCommDistSquared = tmpReal * tmpReal;

	gProperties.checkAndGetPropertyValue("gColorChangeTarget", &tmpInt, true);
	_colorChangeTarget = tmpInt;
	gProperties.checkAndGetPropertyValue("gColorChangeIteration", &tmpInt, true);
	_colorChangeIteration = tmpInt;

	gUseDitchSensors = false;

	G_COLOR_WHITE = SDL_MapRGB(gForegroundImage->format, 0xFF, 0xFF, 0xFF);

	// initialize squared distance matrix
	gDistSquared.assign(gAgentCounter * gAgentCounter, .0);
	//for (int i = 0; i != gAgentCounter; i++) (gDistSquared.at(i)).reserve(gAgentCounter);

	// initialize communication network data
	gBoolRadioNetworkArray.assign(gAgentCounter * gAgentCounter, true);
	//for (int i = 0; i != gAgentCounter; i++) (gRadioNetworkArray.at(i)).reserve(gAgentCounter);


	if (gEnergyMode) {
		// Randomly place pucks on the field.
		for (int i = 0; i < gPuckCount; i++) {
			gPucks[i].replace(false);
		}
	}


	timer = new Timer();
	timer->start();
}

void MONEEWorldObserver::step()
{
	int xa, ya, xb, yb, xo, yo, ds;

//	std::vector<RobotAgentPtr>* agents = _world->listAgents();
// Please use _world->getAgent instead of this.
	for (int i = 0; i < gAgentCounter - 1; i++) {
		for (int j = i + 1; j < gAgentCounter; j++) {
			_world->getAgent(i)->getCoord(xa, ya);
			_world->getAgent(j)->getCoord(xb, yb);
			xo = xb - xa;
			yo = yb - ya;
			ds = xo * xo + yo * yo;
			gDistSquared[i * gAgentCounter + j] = ds;//gDistSquared[j * gAgentCounter + i] = ds;
			gBoolRadioNetworkArray[i * gAgentCounter + j] = (ds <= gMaxCommDistSquared);//gRadioNetworkArray[j * gAgentCounter + i] = (ds <= gMaxCommDistSquared);
		}
	}

    int iteration = _world->getIterations();
    if (iteration == _colorChangeIteration) {
    	doColorChange();
    }

    if (iteration % 1000 == 0) {
        double progress = (double)iteration / (double)gMaxIt * 100.0;
        std::cout << std::endl << "##########" << std::endl << "# ";
        std::cout.width(5); std::cout.precision(1);
        std::cout << std::fixed << progress;
        std::cout << "% #" << std::endl << "##########" << std::endl << std::endl;
    }

	if (_world->getIterations() == gMaxIt - 1) {
		printf("Took %d ms.\n", timer->get_ticks());
	}
}


