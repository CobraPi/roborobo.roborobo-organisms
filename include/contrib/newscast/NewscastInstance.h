/*
 *  NewscastInstance.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef NEWSCASTINSTANCE_H
#define NEWSCASTINSTANCE_H 

#include <iostream>
#include <list>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/NeuralController.h"

#include "newscast/PopulationMember.h"
#include "newscast/NewsItem.h"

using namespace std;

class NewscastInstance : public PopulationMember
{
public:
	NewscastInstance(unsigned int cacheSize, int itemTTL, int currentTime);
	virtual ~NewscastInstance();
	
	void update(int currentTime); // the active algorithm
	list<NewsItem *> exchange(list<NewsItem *> otherCache, int currentTime); // the passive algorithm
	virtual NewsItem *getNews() = 0; // get news from the agent
	virtual void newsUpdate(list<NewsItem *> allItems) = 0; // send news to the agent
	
	int getId();
	
protected:
	static bool compareByAgentIdAndTimestamp(NewsItem *first, NewsItem *second);
	//static bool compareByTimestamp(NewsItem *first, NewsItem *second);
	static bool compareByTimestampReversed(NewsItem *first, NewsItem *second);
	static bool sameAgentIdAndTimestamp(NewsItem *first, NewsItem *second);
	static bool sameAgentId(NewsItem *first, NewsItem *second);
	
private:
	static vector<NewscastInstance *> allCorrespondents;
	
	int myId, itemTTL;
	unsigned int cacheSize;
	list<NewsItem *> myCache;
	
	void removeItemsOlderThan(int timestamp);
	void commonPreSend(int currentTime);
	void commonPostSend(list<NewsItem *> otherCache);
	
};


#endif
