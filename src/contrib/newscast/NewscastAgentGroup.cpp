/*
 *  NewscastAgentGroup.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-03-11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "newscast/NewscastAgentGroup.h"

int NewscastAgentGroup::nextId = 0;

NewscastAgentGroup::NewscastAgentGroup(int cacheSize, int itemTTL, int memorySize) {
	myId = nextId++;

	this->cacheSize = cacheSize;
	this->itemTTL = itemTTL;
	this->memorySize = memorySize;
	this->agents = vector<NewscastAgent *>();
}

NewscastAgentGroup::~NewscastAgentGroup() {
	this->agents.clear(); // desctructs the agents
}

void NewscastAgentGroup::update(int currentTime) {
	for (unsigned int i = 0; i < agents.size(); i++) {
		agents[i]->update(currentTime);
	}
}

int NewscastAgentGroup::getId() {
	return myId;
}

int NewscastAgentGroup::getActiveCount() {
	int result = 0;
	
	for (unsigned int i = 0; i < this->agents.size(); i++) {
		if (agents[i] != NULL && agents[i]->getActiveCount() > result) {
			result = agents[i]->getActiveCount();
		}
	}
	
	return result;
}

void NewscastAgentGroup::setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime) {
	if (agents.size() <= ctrlNr) {
		agents.resize(ctrlNr+1, NULL);
	}
	if (agents[ctrlNr] == NULL) {
		agents[ctrlNr] = new NewscastAgent(cacheSize, itemTTL, currentTime, memorySize);
	}
	agents[ctrlNr]->setNews(controller, ctrlNr, currentTime);
}

NewsItem *NewscastAgentGroup::randomNews() {
	return randomNews(ControllerPtr(), ControllerPtr());
}

NewsItem *NewscastAgentGroup::randomNews(ControllerPtr excludeA) {
	return randomNews(excludeA, ControllerPtr());
}

NewsItem *NewscastAgentGroup::randomNews(ControllerPtr excludeA, ControllerPtr excludeB) {
	NewsItem *pickedItem = NULL;
	
	if (getActiveCount() == 0) {
		if (debug) { cout << "WARNING: News requested from group while no agent had set news yet" << endl; }
		return NULL;
	}
	
	if (getActiveCount() == 1 && (excludeA || excludeB)) {
		if (debug) { cout << "WARNING: Second news requested from group while only one agent has set news" << endl; }
		return NULL;
	}
	
	if (getActiveCount() == 2 && excludeA && excludeB) {
		if (debug) { cout << "WARNING: Third news requested from group while only two agents have set news" << endl; }
		return NULL;
	}
	
	/* Select one of the agents and ask ask it for a random news item */
	while (pickedItem == NULL ||
		   (pickedItem != NULL && pickedItem->getContent() != NULL && (pickedItem->getContent() == excludeA || pickedItem->getContent() == excludeB))) {
		int pickedAgent = Rand::randint(0, agents.size());
		if (agents[pickedAgent] != NULL && agents[pickedAgent]->getActiveCount() > 0) {
			pickedItem = agents[pickedAgent]->randomNews();
		}
	}
	
	return pickedItem;
}

NewsItem *NewscastAgentGroup::randomNews(vector<ControllerPtr> *exclude){
        NewsItem *pickedItem = NULL;
	
	if (getActiveCount() == 0) {
		if (debug) { cout << "WARNING: News requested from group while no agent had set news yet" << endl; }
		return NULL;
	}
        
	if((unsigned)getActiveCount() <= exclude->size()) {
            if (debug) { cout << "WARNING: " << (exclude->size()+1) << " news items set while " << getActiveCount() << " available"; }
            return NULL;
        }
        
	/* Select one of the agents and ask ask it for a random news item */
	while (pickedItem == NULL || (pickedItem != NULL && pickedItem->getContent() != NULL && Util::checkExcludesForItem(pickedItem, exclude))) {
		int pickedAgent = Rand::randint(0, agents.size());
		if (agents[pickedAgent] != NULL && agents[pickedAgent]->getActiveCount() > 0) {
			pickedItem = agents[pickedAgent]->randomNews();
		}
	}
	return pickedItem;
}
