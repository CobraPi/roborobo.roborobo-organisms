#include "Utils/Controller.h"

int Controller::nextId = 0;
double Controller::initialMutationStepSize = 5.0; // should be overwritten by properties file
double Controller::controllerMutationStepSizeMinimum = 0.001;

Controller::Controller(){
	this->fitness = -1;
	this->originalFitness = -1;
	this->id = nextId++;
	this->mutationStepSizes = vector<double>(0);
}

Controller::Controller(vector<double> _mutationStepSizes){
	this->fitness = -1;
	this->originalFitness = -1;
	this->id = nextId++;
	this->mutationStepSizes = vector<double>(_mutationStepSizes);
}

Controller::~Controller(){
	// nothing to do
}

bool Controller::compare(ControllerPtr one, ControllerPtr other) {
	if (one == NULL) {
		return false;
	}
	if (other == NULL) {
		return true;
	}
	return one->GetFitness() > other->GetFitness();
}

void Controller::SetFitness(double fitness) {
	this->fitness = fitness;
	this->originalFitness = fitness;
}

double Controller::GetFitness() {
	return fitness;
}

void Controller::SetOriginalFitness(double originalFitness) {
	this->originalFitness = originalFitness;
}

double Controller::GetOriginalFitness() {
	return originalFitness;
}

void Controller::SetMutationStepSize(int i, double stepSize) {
	this->mutationStepSizes[i] = stepSize;
}

double Controller::GetMutationStepSize(int i) {
	return this->mutationStepSizes[i];
}

string Controller::ToString() {
	ostringstream os;
	os << "controller ";
	os << id;
	os << endl;
	
	os << endl << "m: ";
	for (unsigned int i = 0; i < mutationStepSizes.size(); i++) {
		if (i > 0) {
			os << ", ";
		}
		os << mutationStepSizes[i];
	}
	
	return os.str();
}

string Controller::ToShortString() {
	ostringstream os;
	os << "controller ";
	os << id;
	return os.str();
}

void Controller::setWorldModel(RobotAgentWorldModel* __wm){
	this->_wm = __wm;
}

void Controller::joinedOrganism(Organism *organism){
	// TODO
}
