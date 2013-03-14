/*
 *  NewscastAgentGroup.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 15-03-11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEWSCASTAGENTGROUP_H
#define NEWSCASTAGENTGROUP_H 

#include <iostream>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "newscast/PopulationMember.h"
#include "newscast/NewscastAgent.h"
#include "newscast/NewsItem.h"

using namespace std;

class NewscastAgentGroup : public PopulationMember
{
public:
	NewscastAgentGroup(int cacheSize, int itemTTL, int memorySize);
	virtual ~NewscastAgentGroup();
	
	virtual void update(int currentTime); // should be called every T timesteps
	
	virtual int getActiveCount();
	virtual void setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime);
	virtual NewsItem *randomNews();
	virtual NewsItem *randomNews(ControllerPtr exclude);
	virtual NewsItem *randomNews(ControllerPtr excludeA, ControllerPtr excludeB);
        virtual NewsItem *randomNews(vector<ControllerPtr> *exclude);
	
	virtual int getId();
	
private:
	static const bool debug = false;
	static int nextId;
	int myId, memorySize, itemTTL;
	unsigned int cacheSize;
	vector<NewscastAgent *> agents;
};

#endif
