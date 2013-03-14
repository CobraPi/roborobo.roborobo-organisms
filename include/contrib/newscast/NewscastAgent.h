/*
 *  NewscastAgent.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEWSCASTAGENT_H
#define NEWSCASTAGENT_H 

#include <iostream>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "newscast/NewscastInstance.h"
#include "newscast/NewsItem.h"

using namespace std;

class NewscastAgent : public NewscastInstance
{
public:
	NewscastAgent(unsigned int cacheSize, int itemTTL, int currentTime, int memorySize);
	~NewscastAgent();
	
	int getActiveCount();
	void setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime);
	vector<NewsItem *> allNews();
	virtual NewsItem *randomNews();
	virtual NewsItem *randomNews(ControllerPtr exclude);
	virtual NewsItem *randomNews(ControllerPtr excludeA, ControllerPtr excludeB);
        virtual NewsItem *randomNews(vector<ControllerPtr > *exclude);
	
	virtual NewsItem *getNews(); // get news from this agent
	virtual void newsUpdate(list<NewsItem *> allItems); // send news to this agent
	
private:
	static const bool debug = false;
	static int nextId;
	int myId;
	unsigned memorySize;
	NewsItem *currentNews;
	vector<NewsItem *> latestItems;
};

#endif
