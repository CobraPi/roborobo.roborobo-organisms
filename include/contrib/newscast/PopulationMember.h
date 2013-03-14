/*
 *  population.h
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 11-11-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef POPULATIONMEMBER_H
#define POPULATIONMEMBER_H 

#include <iostream>
#include <list>

#include "Utils/Rand.h"
#include "Utils/Util.h"
#include "Utils/Controller.h"

#include "newscast/NewsItem.h"

using namespace std;

class PopulationMember
{
public:
	PopulationMember();
	virtual ~PopulationMember();
	
	virtual void update(int currentTime); // should be called every T timesteps

	virtual int getActiveCount() = 0;
	virtual void setNews(ControllerPtr controller,unsigned int ctrlNr, int currentTime) = 0;
	virtual NewsItem *randomNews() = 0;
	virtual NewsItem *randomNews(ControllerPtr exclude) = 0;
	virtual NewsItem *randomNews(ControllerPtr excludeA, ControllerPtr excludeB) = 0;
    virtual NewsItem *randomNews(vector<ControllerPtr> *exclude) = 0;
	
	virtual int getId() = 0;
};


#endif
