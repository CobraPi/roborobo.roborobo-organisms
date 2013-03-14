/*
 *  HiveMindInstance.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "newscast/HiveMindInstance.h"

vector<vector<NewsItem *> *> HiveMindInstance::allItems = vector<vector<NewsItem *> *> ();

HiveMindInstance::HiveMindInstance() :
	PopulationMember() {
	myId = -1;
}

HiveMindInstance::~HiveMindInstance() {
	for (uint i = 0; i < allItems.size(); i++) {
		// null checks are here because the destructor gets called twice
		// for some reason
		if (allItems[i] != NULL) {
			for (uint j = 0; j < allItems[i]->size(); j++) {
				if (allItems[i]->at(j) != NULL) {
					delete allItems[i]->at(j);
					allItems[i]->at(j) = NULL;
				}
			}
			delete allItems[i];
			allItems[i] = NULL;
		}
	}
}

int HiveMindInstance::getId() {
	return myId;
}

int HiveMindInstance::getActiveCount() {
	return allItems.size();
}

void HiveMindInstance::setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime) {
	if (myId == -1) {
		// this instance is about to set its first news
		// create the vector to store my news items
		myId = allItems.size();
		vector<NewsItem *> *myItems = new vector<NewsItem *> ();
		allItems.push_back(myItems);

		// store the news item
		if (myItems->size() <= ctrlNr) {
			myItems->resize(ctrlNr + 1, NULL);
		}
		NewsItem *item = new NewsItem(currentTime, controller, myId);
		myItems->at(ctrlNr) = item;
	} else {
		// this instance is adding a new piece of news or replacing an old piece of news
		vector<NewsItem *> *myItems = allItems[myId];
		if (myItems->size() <= ctrlNr) {
			// this news is coming into a new slot, create the slot
			myItems->resize(ctrlNr + 1, NULL);
		} else {
			// this news is replacing and old piece of news, remove that old news
			delete myItems->at(ctrlNr);
			myItems->at(ctrlNr) = NULL;
		}
		// add the new news
		NewsItem *item = new NewsItem(currentTime, controller, myId);
		myItems->at(ctrlNr) = item;
	}
}

vector<NewsItem *> HiveMindInstance::allNews() {
	vector<NewsItem *> result = vector<NewsItem *> ();
	for (unsigned int i = 0; i < allItems.size(); i++) {
		for (unsigned int j = 0; j < allItems[i]->size(); j++) {
			result.push_back(allItems[i]->at(j));
		}
	}

	return result;
}

NewsItem *HiveMindInstance::randomNews() {
	return randomNews(ControllerPtr(), ControllerPtr());
}

NewsItem *HiveMindInstance::randomNews(ControllerPtr excludeA) {
	return randomNews(excludeA, ControllerPtr());
}

NewsItem *HiveMindInstance::randomNews(ControllerPtr excludeA, ControllerPtr excludeB) {
	int pickedNews = -1;
	NewsItem *pickedItem = NULL;

	if (getActiveCount() == 0) {
		cout << "WARNING: News requested while no agent had set news yet" << endl;
		return NULL;
	}

	if (getActiveCount() == 1 && (excludeA != NULL || excludeB != NULL)) {
		cout << "WARNING: Second news requested while only one agent has set news" << endl;
		return NULL;
	}

	if (getActiveCount() == 2 && excludeA != NULL && excludeB != NULL) {
		cout << "WARNING: Third news requested while only two agents have set news" << endl;
		return NULL;
	}

	/* Select a random news item that is not one of the exclude arguments */
	while (pickedItem == NULL || (pickedItem != NULL && (pickedItem->getContent() == excludeA || pickedItem->getContent() == excludeB))) {
		// select a news item
		int pickedPeer = Rand::randint(0, allItems.size());
		pickedNews = Rand::randint(0, allItems[pickedPeer]->size());
		pickedItem = allItems[pickedPeer]->at(pickedNews);
	}
	return pickedItem;
}

NewsItem *HiveMindInstance::randomNews(vector<ControllerPtr> *exclude) {
	int pickedNews = -1;
	NewsItem *pickedItem = NULL;

	if (getActiveCount() == 0) {
		cout << "WARNING: News requested while no agent had set news yet" << endl;
		return NULL;
	}

	if (getActiveCount() == 1 && ((exclude->size() >= 2 && (exclude->at(0) != NULL || exclude->at(1) != NULL)) || (exclude->size() >= 1 && exclude->at(0)
			!= NULL))) {
		cout << "WARNING: Second news requested while only one agent has set news" << endl;
		return NULL;
	}

	if (getActiveCount() == 2 && exclude->size() >= 2 && exclude->at(0) != NULL && exclude->at(1) != NULL) {
		cout << "WARNING: Third news requested while only two agents have set news" << endl;
		return NULL;
	}

	/* Select a random news item that is not one of the exclude arguments */
	while (pickedItem == NULL || (pickedItem != NULL && Util::checkExcludesForItem(pickedItem, exclude))) {
		// select a news item
		int pickedPeer = Rand::randint(0, allItems.size());
		pickedNews = Rand::randint(0, allItems[pickedPeer]->size());
		pickedItem = allItems[pickedPeer]->at(pickedNews);
	}
	return pickedItem;
}
