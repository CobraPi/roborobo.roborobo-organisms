/*
 *  Util.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * Util.h
 *
 *  Created on: Jun 10, 2010
 *      Author: rjh
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "Rand.h"
#include "Controller.h"
#include "NeuralController.h"
#include "newscast/NewsItem.h"
#include "WorldModels/RobotAgentWorldModel.h"
#include <cmath>
#include <algorithm>

class NewsItem;
class Util {

	public:
		Util();
		virtual ~Util();

		static void mutate(NeuralController *victim);
		static NeuralControllerPtr avgCrossover(NeuralControllerPtr parent1, NeuralControllerPtr parent2);
		static NeuralControllerPtr avgMultiParentCrossover(vector<NeuralControllerPtr> *parents);
		static NeuralControllerPtr nPointMultiParentCrossover(vector<NeuralControllerPtr> *parents);
		static ControllerPtr randomController(vector<ControllerPtr> *candidates);
		//static Controller *binaryTournament(vector<Controller *> *candidates, Controller *exclude);
		static ControllerPtr binaryTournament(vector<ControllerPtr> *candidates, vector<ControllerPtr> *exclude);
		static NeuralControllerPtr createRandomGenome(RobotAgentWorldModel *__wm);

		static double avgPositiveOnly(vector<double> *list);
		static bool checkExcludesForItem(NewsItem *item, vector<ControllerPtr> *exclude);
		static bool checkExcludesForController(ControllerPtr xController, vector<ControllerPtr> *exclude);

};

#endif /* UTIL_H_ */
