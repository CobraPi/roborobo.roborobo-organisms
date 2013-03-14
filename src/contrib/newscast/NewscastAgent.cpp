/*
 *  NewscastAgent.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "newscast/NewscastAgent.h"

int NewscastAgent::nextId = 0;

NewscastAgent::NewscastAgent(unsigned int cacheSize, int itemTTL, int currentTime, int memorySize) : NewscastInstance(cacheSize, itemTTL, currentTime) {
	myId = nextId++;
	this->memorySize = memorySize;
	latestItems = vector<NewsItem *>();
	currentNews = new NewsItem(currentTime, ControllerPtr(), myId);
}

NewscastAgent::~NewscastAgent() {
	// nothing here
}

/** 
 * This method is called by the evolutionary algorithm, to set a new active controller
 * This will also clear the cache of known news items; update must be called before new
 * news items are guaranteed to be retrievable.
 *
 * The ctrlNr parameter is currently ignored
 */
void NewscastAgent::setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime) {
	delete currentNews;
	// Intentionally ignores 'ctrlNr'
	currentNews = new NewsItem(currentTime, controller, myId);
}

/**
 * This method is called by the newscast algorithm, to get the current news item for this agent
 */
NewsItem *NewscastAgent::getNews() {
	return currentNews->clone();
}

/**
 * This method is called by the newscast algorithm, to notify the agent of new news 
 */
void NewscastAgent::newsUpdate(list<NewsItem *> nwItems) {
	// add the new items to the list of latest items
	for (list<NewsItem *>::iterator iter = nwItems.begin(); iter != nwItems.end(); iter++) {
		if (std::find(latestItems.begin(), latestItems.end(), *iter) != latestItems.end()) {
			latestItems.push_back(*iter);
		}
	}
	
	// keep only the newest item of each agent
	std::sort(latestItems.begin(),latestItems.end(),compareByAgentIdAndTimestamp);
	std::unique(latestItems.begin(), latestItems.end());
	
	// keep only the 'memorySize' newest items
	std::sort(latestItems.begin(),latestItems.end(),compareByTimestampReversed); // larger timestamps towards the head of the list
	if (latestItems.size() > memorySize) {
		latestItems.resize(memorySize);
	}
}

int NewscastAgent::getActiveCount() {
	return latestItems.size();
}

vector<NewsItem *> NewscastAgent::allNews() {
	vector<NewsItem *> result = vector<NewsItem *>();
	for (vector<NewsItem *>::iterator iter = latestItems.begin(); iter != latestItems.end(); iter++) {
		result.push_back(*iter);
	}
	return result;
}

NewsItem *NewscastAgent::randomNews() {
	return randomNews(ControllerPtr(),ControllerPtr());
}

NewsItem *NewscastAgent::randomNews(ControllerPtr excludeA) {
	return randomNews(excludeA, ControllerPtr());
}

NewsItem *NewscastAgent::randomNews(ControllerPtr excludeA, ControllerPtr excludeB) {
	int pickedNews = -1;
	NewsItem *pickedItem = NULL;
	
	if (latestItems.size() == 0) {
		if (debug) { cout << "WARNING: News requested while no agent had set news yet" << endl; }
		return NULL;
	}
	
	if (latestItems.size() == 1 && (excludeA || excludeB)) {
		if (debug) { cout << "WARNING: Second news requested while only one agent has set news" << endl; }
		return NULL;
	}
	
	if (latestItems.size() == 2 && excludeA && excludeB) {
		if (debug) { cout << "WARNING: Third news requested while only two agents have set news" << endl; }
		return NULL;
	}
	
	/* Select a random news item that is not one of the exclude arguments */
	while (pickedItem == NULL ||
		   (pickedItem != NULL && pickedItem->getContent() != NULL && (pickedItem->getContent() == excludeA || pickedItem->getContent() == excludeB))) {
		// select a news item
		pickedNews = Rand::randint(0, latestItems.size());
		pickedItem = latestItems[pickedNews];
	}
	return pickedItem;
}

NewsItem *NewscastAgent::randomNews(vector<ControllerPtr > *exclude){
        int pickedNews = -1;
	NewsItem *pickedItem = NULL;
	
	if (latestItems.size() == 0) {
		if (debug) { cout << "WARNING: News requested while no agent had set news yet" << endl; }
		return NULL;
	}
	
        if(latestItems.size() <= exclude->size()) {
            if (debug) { cout << "WARNING: " << (exclude->size()+1) << " news items set while " << latestItems.size() << " available"; }
            return NULL;
        }
	
	/* Select a random news item that is not one of the exclude arguments */
	while (pickedItem == NULL || (pickedItem != NULL && pickedItem->getContent() != NULL && Util::checkExcludesForItem(pickedItem, exclude))) {
            pickedNews = Rand::randint(0, latestItems.size());
            pickedItem = latestItems[pickedNews];
	}
	return pickedItem;
}
