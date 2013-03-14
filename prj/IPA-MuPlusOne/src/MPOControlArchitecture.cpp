/*
 *  MPOControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "IPA-MuPlusOne/include/MPOControlArchitecture.h"

unsigned int MPOControlArchitecture::mu = 3; // should be overwritten by properties file
double MPOControlArchitecture::reEvaluationRate = 0.1; // should be overwritten by properties file
double MPOControlArchitecture::crossoverRate = 1.0; // should be overwritten by properties file
double MPOControlArchitecture::mutationRate = 1.0; // should be overwritten by properties file
int MPOControlArchitecture::id = 0;
int MPOControlArchitecture::numberOfParents = 2; // should be overwritten by properties file

MPOControlArchitecture::MPOControlArchitecture(RobotAgentWorldModel *__wm) :
	EvolutionaryControlArchitecture(__wm) {
	myId = ++MPOControlArchitecture::id;
}

MPOControlArchitecture::~MPOControlArchitecture() {
	// nothing to do.
}

ControllerPtr MPOControlArchitecture::getNextCandidate() {

	ControllerPtr candidate;

	// if the hall of fame is full, mutate an existing controller or re-evaluate one to create a new candidate
	if (hallOfFame.size() == mu) {
		// decide whether to re-evaluate or mutate a controller
		if (Rand::randouble() < reEvaluationRate) { // re-evaluate
			candidate = Util::binaryTournament(&hallOfFame, NULL);
			if (debug) {
				cout << "Selected re-evaluation of " << candidate->ToShortString() << endl;
			}
		} else { // mutate existing
			// parent selection
			ControllerPtr parentA = Util::binaryTournament(&hallOfFame, NULL);

			// crossover or clone
			if (Rand::randouble() < crossoverRate) {
				vector<ControllerPtr> *parents = new vector<ControllerPtr> ();
				parents->push_back(parentA);
				if (debug) {
					cout << "parentSize " << parents->size() << " " << numberOfParents << endl;
				}
				while (parents->size() < (unsigned) numberOfParents) {
					if (debug) {
						cout << parents->size() << endl;
					}
					parents->push_back(Util::binaryTournament(&hallOfFame, parents));
				}
				if (debug) {
					cout << parents->size() << endl;
				}
				// create a candidate from crossover between the parents
				candidate = parentA->crossOverWithMP(parents);

				//Controller *parentB = Util::binaryTournament(&hallOfFame, parentA);
				//candidate = parentA->crossOverWith(parentB);
				if (debug) {
					cout << "parentSize " << parents->size() << " Created " << candidate->ToShortString() << " from crossover between "
							<< parents->at(0)->ToShortString();
					for (uint i = 1; i < parents->size(); i++)
						cout << " and " << parents->at(i)->ToShortString();
					cout << endl;
				}
				parents->clear();
				delete parents;
				// System.out.println("Chose parents: " + parentA + " and " +
				// parentB);
			} else {
				candidate = parentA->Clone();
				if (debug) {
					cout << "Spawned " << candidate->ToShortString() << " from " << parentA->ToShortString() << endl;
				}
			}

			// mutation
			if (Rand::randouble() < mutationRate) {
				//cout << "Before mutation: " << candidate->ToString() << endl;
				candidate->mutate();
				//cout << "After mutation: " << candidate->ToString() << endl;
			}

			if (debug) {
				cout << "Mutated " << candidate->ToShortString() << endl;
			}
		}
	}

	// if the hall of fame is not yet full, generate a new candidate randomly
	if (hallOfFame.size() < mu) {
		candidate = Util::createRandomGenome(_wm);
		if (debug) {
			cout << "Generated " << candidate->ToString() << endl;
		}
	}

	return candidate;

}

void MPOControlArchitecture::setCandidateFitness(ControllerPtr candidate, double scoredFitness) {

	// running evaluation has completed. Place candidate in hall of fame.
	if (candidate->GetFitness() > -1) {
		// this is a re-evaluation, average out the previous and the new score
		if (debug) {
			cout << "This was a re-evaluation, not adding a new candidate" << endl;
		}
		candidate->SetFitness((candidate->GetFitness() + scoredFitness) / 2);
		if (debug || verbose) {
			cout << "Finished re-evaluating " << candidate->ToShortString() << " - fitness: " << scoredFitness << ", avg: " << candidate->GetFitness() << endl;
		}
	} else {
		// this was a first evaluation, add to the bottom of the hall of fame
		if (debug) {
			cout << "This was first evaluation, adding a new candidate" << endl;
		}
		candidate->SetFitness(scoredFitness);
		hallOfFame.push_back(candidate);
		if (debug || verbose) {
			cout << "Finished evaluating " << candidate->ToShortString() << " - fitness: " << scoredFitness << endl;
		}
	}
	if (!debug && !verbose) {
		// there has been no output on the fitness yet, do that now
		cout << scoredFitness << endl;
	}

	// order the hall of fame - if there are too many entries, remove the worst-scoring one
	sort(hallOfFame.begin(), hallOfFame.begin() + hallOfFame.size(), Controller::compare);
	if (hallOfFame.size() > mu) {
		hallOfFame.pop_back();
	}
	if (debug) {
		cout << " --- HOF #: " << (rankOf(candidate) + 1) << " of " << hallOfFame.size() << endl;
	}

}

int MPOControlArchitecture::rankOf(ControllerPtr ctrl) {
	vector<ControllerPtr>::iterator result = find(hallOfFame.begin(), hallOfFame.end(), ctrl);
	if (result == hallOfFame.end()) {
		return -1;
	}
	return result - hallOfFame.begin();
}

