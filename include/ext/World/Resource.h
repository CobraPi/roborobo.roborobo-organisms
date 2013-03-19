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

class Resource {
protected:
	Point2d position;
	Uint32 color;

public:
	virtual ~Resource();

    /**
     * Display the resource on a surface.
     *
     * @param surface SDL_Surface to display on
     * @param onScreen if true will also display the resource on the screen
     *
     */
	virtual void display(SDL_Surface *surface) = 0;

    /**
     * hide the resource on a surface.
     *
     * @param surface SDL_Surface to display on
     * @param onScreen if true will also hide the resource on the screen
     *
     */
	virtual void hide(SDL_Surface *surface) = 0;

	virtual void setPosition(Point2d position);

	virtual Point2d getPosition();

	Uint32 getColor();

	void setColor(Uint32 color);

	virtual void step(SDL_Surface *surface) = 0;
};




#endif /* RESOURCE_H_ */
