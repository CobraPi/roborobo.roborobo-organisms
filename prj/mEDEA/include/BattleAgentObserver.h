/*
 *  BattleAgentObserver.h
 *  Roborobo
 *
 *  Created by Jean-Marc on 15/12/09
 *
 */

#ifndef BATTLE_AGENT_OBSERVER_H
#define BATTLE_AGENT_OBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/AgentObserver.h"

#include "mEDEA/include/MedeaAgentWorldModel.h"

#include "mEDEA/include/MedeaSharedData.h"

class BattleAgentObserver : public AgentObserver
{
	protected:
		MedeaAgentWorldModel	*_wm;

		std::map<int, std::vector<double> > _genomesList;
		std::map<int, float > _sigmaList;
		std::vector<double> _currentGenome;
		float _currentSigma;

		double _minValue;
		double _maxValue;
		int _nbGene;
		int _nbLayer;
		int _nbHiddenNeurons;

		int _evaluationCount;
		int _generationCount;
		int _iterationCount;
		bool _synchronization;

		void pickRandomGenome();
		void logStatus();

		int _side;
		float _key;

	public:
		BattleAgentObserver( RobotAgentWorldModel *__wm );
		~BattleAgentObserver();
				
		void reset();
		void step();

		void writeGenome(std::vector<double> genome, int senderId);
		void loadGenome ( std::vector<double> inGenome );

		int getSide();
};

#endif
