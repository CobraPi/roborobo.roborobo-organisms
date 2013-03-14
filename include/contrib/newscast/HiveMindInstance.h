/*
 *  HiveMindInstance.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef HIVEMINDINSTANCE_H
#define HIVEMINDINSTANCE_H 

#include <iostream>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "newscast/NewsItem.h"
#include "newscast/PopulationMember.h"

using namespace std;

class HiveMindInstance : public PopulationMember
{
public:
	HiveMindInstance();
	virtual ~HiveMindInstance();
	
	virtual int getActiveCount();
	virtual void setNews(ControllerPtr controller, unsigned int ctrlNr, int currentTime);
	vector<NewsItem *> allNews();
	virtual NewsItem *randomNews();
	virtual NewsItem *randomNews(ControllerPtr exclude);
	virtual NewsItem *randomNews(ControllerPtr excludeA, ControllerPtr excludeB);
    virtual NewsItem *randomNews(vector<ControllerPtr> *exclude);

	virtual int getId();
	
protected:
	int myId;
	static vector<vector<NewsItem *> *> allItems;
	
};


#endif
