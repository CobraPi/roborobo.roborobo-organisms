/*
 *  NewsItem.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 08-10-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "newscast/NewsItem.h"


NewsItem::NewsItem(int timestamp, ControllerPtr content, int agentId) {
	this->timestamp = timestamp;
	this->content = content;
	this->agentId = agentId;
}

NewsItem::~NewsItem() {
}

int NewsItem::getTimestamp() {
	return timestamp;
}

ControllerPtr NewsItem::getContent() {
	return content;
}

int NewsItem::getAgentId() {
	return agentId;
}

NewsItem *NewsItem::clone() {
	return new NewsItem(timestamp, content, agentId);
}
