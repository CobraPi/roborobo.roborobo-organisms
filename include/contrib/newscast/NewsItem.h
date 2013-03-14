/*
 *  NewsItem.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef NEWSITEM_H
#define NEWSITEM_H 

#include <iostream>
#include <map>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "newscast/NewsContent.h"

using namespace std;

class NewsItem
{
public:
	NewsItem(int timestamp, ControllerPtr content, int agentId);
	~NewsItem();
	
	int getTimestamp();
	ControllerPtr getContent();
	int getAgentId();
	
	NewsItem *clone();
	
private:
	
	int timestamp;
	ControllerPtr content;
	int agentId;
};


#endif

