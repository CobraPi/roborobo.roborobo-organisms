/*
 *  NewscastInstance.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "newscast/NewscastInstance.h"

vector<NewscastInstance *> NewscastInstance::allCorrespondents = vector<NewscastInstance *>();

NewscastInstance::NewscastInstance(unsigned int cacheSize, int itemTTL, int currentTime) {
	this->cacheSize = cacheSize;
	this->itemTTL = itemTTL;
	myId = allCorrespondents.size();
	myCache = list<NewsItem *>();

	// populate the cache with an initial content of at most 'cacheSize' other agents
	for (unsigned int i = 0; i < allCorrespondents.size() && i < cacheSize; i++) {
		myCache.push_back(new NewsItem(currentTime, ControllerPtr(), i));
	}
	
	// publish myself as an active agent
	allCorrespondents.push_back(this);
}

NewscastInstance::~NewscastInstance() {
	while(!myCache.empty()){
		delete myCache.front();
		myCache.pop_front();
	}
}

int NewscastInstance::getId() {
	return myId;
}

/**
 * this is the active part of the newscast algorithm. It should be called every T timesteps.
 * See Newscast Internal Report page 11 for the outline of this algorithm
 */
void NewscastInstance::update(int currentTime) {
	// 0a. Do not do anything if the cache is empty
	if (myCache.size() == 0) { return; }
	
	// 0b. Do not do anything if this node is the only entry in the cache
	if (myCache.size() == 1
		&& allCorrespondents.at((*(myCache.begin()))->getAgentId()) == this)
	{
		return;
	}
		
	// 1. Pick a random peer from the cache
	NewscastInstance *pickedPeer = NULL;
	do {
		// 1.1. decide which entry from the cache to pick
		int pick = Rand::randint(0, myCache.size());
		
		// 1.2. find the picked entry in the cache	
		list<NewsItem *>::iterator it = myCache.begin();
		for (int i = 0; i < pick; i++) {
			it++;
		}
		NewsItem *pickedItem = *it;

		// 1.3. find the peer that corresponds to that entry in the cache
		pickedPeer = allCorrespondents.at(pickedItem->getAgentId());
	} while (pickedPeer == this);
	
	// 2+3
	commonPreSend(currentTime);
	
	// 4+5. Send my cache to the chosen peer, receive its cache
	list<NewsItem *> cacheToSend = list<NewsItem *>();
	for (list<NewsItem *>::iterator it = myCache.begin(); it != myCache.end(); it++) {
		cacheToSend.push_back((*it)->clone());
	}
	list<NewsItem *> otherCache = pickedPeer->exchange(cacheToSend, currentTime);
		
	// 6+7+8+9
	commonPostSend(otherCache);
}

/**
 * this is the passive part of the newscast algorithm. It does not need to be called by the user.
 * See Newscast Internal Report page 11 for the outline of this algorithm
 */
list<NewsItem *> NewscastInstance::exchange(list<NewsItem *> otherCache, int currentTime) {
	// 2+3
	commonPreSend(currentTime);
	
	// 4. Copy the current version of my cache for reply to the calling peer
	list<NewsItem *> cacheToSend = list<NewsItem *>();
	for (list<NewsItem *>::iterator it = myCache.begin(); it != myCache.end(); it++) {
		cacheToSend.push_back((*it)->clone());
	}
	
	// 6+7+8+9
	commonPostSend(otherCache);
	
	// Return the old version of my cache to the calling peer
	return cacheToSend;
}

/**
 * Operations that are common between the active and passive parts of the algorithm, to be
 * performed before the caches have been exchanged.
 */
void NewscastInstance::commonPreSend(int currentTime) {
	// 2. Get the most recent news from the agent
	myCache.push_back(getNews());
	
	// 2.1 remove duplicate items
	myCache.sort(compareByAgentIdAndTimestamp);
	myCache.unique(sameAgentIdAndTimestamp);
	
	// 3. Remove items that are too old from the cache
	removeItemsOlderThan(currentTime - itemTTL);
}

/**
 * Operations that are common between the active and passive parts of the algorithm, to be
 * performed after the caches have been exchanged.
 */
void NewscastInstance::commonPostSend(list<NewsItem *> otherCache) {
	// 6. Notify the agent of the arrival of news
	this->newsUpdate(otherCache);
	
	// 7. Add received items to the local cache
	for (list<NewsItem *>::iterator it = otherCache.begin(); it != otherCache.end(); it++) {
		myCache.push_back(*it);
	}
	
	// 8. Keep the newst item only from each agent
	myCache.sort(compareByAgentIdAndTimestamp);
	myCache.unique(sameAgentId);
	
	// 9. Keep the 'cacheSize' freshest items
	myCache.sort(compareByTimestampReversed); // larger timestamps towards the head of the list
	if (myCache.size() > cacheSize) {
		myCache.resize(cacheSize);
	}
}

/**
 * Remove items from my cache that are older than the given timestamp
 */
void NewscastInstance::removeItemsOlderThan(int timestamp) {
	// return if the cache is empty
	if (myCache.begin() == myCache.end()) { return; }
	
	// run backwards through the list, removing items that are too old
	list<NewsItem *>::iterator it = myCache.end();
	
	do {
		it--; // move one item forward in the list
		NewsItem *item = *it;
		if (item->getTimestamp() < timestamp) {
			// the item is too old, remove it.
			// the iterator moves to the item after the removed item
			delete item;
			it = myCache.erase(it);
		}
	} while (it != myCache.begin());
}

/**
 * Compare two NewsItem instances for ordering. Returns true if 'first' has an agentID that is
 * smaller than the agentID of 'second'. If the agentIDs are the same, this method
 * returns true if the timestamp of 'first' is smaller than 'second'. Returns false
 * if both agentID and timestamp are identical.
 */
bool NewscastInstance::compareByAgentIdAndTimestamp(NewsItem *first, NewsItem *second) {
	// compare by agent ID
	bool val = first->getAgentId() < second->getAgentId();
	
	// if agent IDs are the same, compare by timestamp, reversed
	if (first->getAgentId() == second->getAgentId()) {
		val = first->getTimestamp() > second->getTimestamp();
	}

	return val;
}

/**
 * Compare two NewsItem instances for ordering, based on timestamp alone. Returns true
 * if the timestamp of the first item is smaller than the timestamp of the second.
 * Returns false otherwise.
 * /
bool NewscastInstance::compareByTimestamp(NewsItem *first, NewsItem *second) {
	return first->getTimestamp() < second->getTimestamp();
}
 */

/**
 * Compare two NewsItem instances for ordering, based on timestamp alone. Returns true
 * if the timestamp of the first item is larger than the timestamp of the second.
 * Returns false otherwise.
 */
bool NewscastInstance::compareByTimestampReversed(NewsItem *first, NewsItem *second) {
	return first->getTimestamp() > second->getTimestamp();
}
				   
/**
 * Compare two NewsItem instances for uniqueness, based only on agent ID. Returns true
 * if the agent IDs of the two given NewsItem instances are the same, false otherwise
 */
bool NewscastInstance::sameAgentIdAndTimestamp(NewsItem *first, NewsItem *second) {
	return (first->getAgentId() == second->getAgentId()) && (first->getTimestamp() == second->getTimestamp());
}
				   
/**
 * Compare two NewsItem instances for uniqueness, based only on agent ID. Returns true
 * if the agent IDs of the two given NewsItem instances are the same, false otherwise
 */
bool NewscastInstance::sameAgentId(NewsItem *first, NewsItem *second) {
	return first->getAgentId() == second->getAgentId();
}
