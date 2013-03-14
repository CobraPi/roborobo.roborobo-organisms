/*
 * Resource.h
 *
 *  Created on: Mar 13, 2013
 *      Author: job
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "RoboroboMain/common.h"
#include "Utilities/Misc.h"

class Resource{
private:
	Point2d position;
	Uint32 color;

public:
	virtual ~Resource();

	virtual void display() = 0;

	virtual void hide() = 0;

	virtual void setPosition(Point2d position);

	virtual Point2d getPosition();

	Uint32 getColor();

	void setColor(Uint32 color);

	virtual void step() = 0;
};




#endif /* RESOURCE_H_ */
