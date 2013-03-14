/*
 * MultiBla.cpp
 *
 *  Created on: 10/10/2011
 *      Author: pgarcia
 */


#include <iostream>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "multikulti/MultiKultiController.h"

#include "multikulti/MultiKultiInstance.h"


using namespace std;

MultiKultiInstance::MultiKultiInstance() :   HiveMindInstance(){
	myId=-1;

}

MultiKultiInstance::~MultiKultiInstance(){
	//cout<<"DESTRUCTOR DE MKINSTANCE!"<<endl;

}
bool comparator(Controller * a, Controller * b){
	//return (a->compare(a,b));

	if (a == NULL) {
		return false;
	}
	if (b == NULL) {
		return true;
	}
	return a->GetOriginalFitness() > b->GetOriginalFitness();
}

void printVector(vector<MultiKultiControllerPtr> v){

	for(unsigned int i = 0; i<v.size();i++){
		double orig = v.at(i)->GetOriginalFitness();
		cout<<"Vector island "<<v.at(i)->ToShortString()<<" ORIG "<<orig<<endl;

	}

}

MultiKultiControllerPtr MultiKultiInstance::getDifferentMigrant(int nodeId, MultiKultiControllerPtr message, int eliteSize){

	if(eliteSize==0)
		eliteSize = 1; //At least one

	int debug = 0;
	vector<MultiKultiControllerPtr> migrants;
	vector<NewsItem*>  * island = allItems[nodeId];

	for(unsigned int i = 0; i<island->size();i++){//Si esto revienta usar allItems->at(i)->blalba
		migrants.push_back(boost::dynamic_pointer_cast<MultiKultiController>(island->at(i)->getContent()));
		//cout<<"RRRRR"<<endl;
		//double orig = island->at(i)->getContent()->GetOriginalFitness();
		//cout<<" Remote island"<<island->at(i)->getContent()->ToShortString()<<" ORIG "<<orig<<endl;
	}

	if(debug){
		cout<<"REMOTE ISLAND:"<<endl;
		printVector(migrants);
	}


	if(eliteSize<island->size() && eliteSize != -1){
		//Sort the vector
		std::sort(migrants.begin(),migrants.end());

		//Get the eliteSize best ones
		migrants.resize(eliteSize);
	}

	if(debug){
		cout<<"SORTED"<<endl;
		printVector(migrants);
	}
	//Get the most different from the elite
	MultiKultiControllerPtr migrant = this->getDifferentMigrantFromVector(message, migrants);
	if(debug)
		cout<<"The migrant has an original fitness of "<<migrant->GetOriginalFitness()<<endl;
	//migrant->SetFitness(migrant->GetOriginalFitness());
	return boost::dynamic_pointer_cast<MultiKultiController>(migrant->Clone());
}

MultiKultiControllerPtr MultiKultiInstance::getRandomMigrant(int nodeId){

	vector<NewsItem*>  * island = allItems[nodeId];
	int i = Rand::randint(0,island->size());
	ControllerPtr migrant = island->at(i)->getContent();

	return boost::dynamic_pointer_cast<MultiKultiController>(migrant->Clone());
}

MultiKultiControllerPtr MultiKultiInstance::getDifferentMigrantFromVector(MultiKultiControllerPtr message, vector<MultiKultiControllerPtr> migrants){

	double maxDifference = 0;
	MultiKultiControllerPtr theMostDifferent = migrants.at(0);
	for(unsigned int i =0; i<migrants.size();i++){
		double actualDifference = calculateDifference(message,migrants.at(i));
		if(actualDifference>maxDifference){
			theMostDifferent = migrants.at(i);
			maxDifference = actualDifference;
		}
	}

	return theMostDifferent; //clone?
}

double MultiKultiInstance::calculateDifference(MultiKultiControllerPtr a, MultiKultiControllerPtr b){
	return a->calculateDifference(b);
	/*NeuralController * na = (NeuralController *)a;
	NeuralController * nb = (NeuralController *)b;

	vector<double> vecA = na->GetWeights();
	vector<double> vecB = nb->GetWeights();

	double difference = 0.0;
	for(unsigned int i = 0; i<vecA.size();i++){
		difference += abs(vecA.at(i) - vecB.at(i));
	}

	return difference/vecA.size();*/
}

MultiKultiControllerPtr MultiKultiInstance::getBestMigrant(int nodeId){

	vector<NewsItem*>  * island = allItems[nodeId];
	//int i = Rand::randint(0,island->size());
	double bestFitness = island->at(0)->getContent()->GetOriginalFitness();
	//cout<<"Island size"<<island->size()<<endl;
	int best = 0;
	//cout<<"0:"<<bestFitness<<endl;
	for(unsigned int i =1;i<island->size();i++){
		double actualFitness = island->at(i)->getContent()->GetOriginalFitness();
		//cout<<i<<":"<<actualFitness<<endl;
		if(actualFitness > bestFitness){
			best = i;
			bestFitness = actualFitness;
			//cout<<"yay"<<endl;
		}

	}
	//cout<<"Received "<<best<<"from node "<<nodeId<<endl;
	ControllerPtr migrant = island->at(best)->getContent();

	return boost::dynamic_pointer_cast<MultiKultiController>(migrant->Clone());
}




