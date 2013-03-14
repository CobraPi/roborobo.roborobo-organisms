/*
 * Resource.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: job
 */
#include "World/Resource.h"

Resource::~Resource(){

}

void Resource::setPosition(Point2d position){
	this->position = position;
}

Point2d Resource::getPosition(){
	return position;
}

Uint32 Resource::getColor(){
	return color;
}

void Resource::setColor(Uint32 color){
	this->color = color;
}




