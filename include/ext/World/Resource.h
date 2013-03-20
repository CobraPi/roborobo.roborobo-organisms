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

/**
 * \class Resource
 *
 * Generic resource class that can be managed by a ResourceFactory and sensed by a ResourceSensor
 *
 * The ResourceSensor detects the red value of color as the type/id of the Resource
 *
 */
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

    /**
     * Set the position of the resource
     *
     * @param position The position to set
     */
	virtual void setPosition(Point2d position);

    /**
     * Get the position of the resource
     *
     * @return Point2d The position
     */
	virtual Point2d getPosition();

    /**
     * Get the color of the resource
     *
     * @return Uint32 The color
     */
	Uint32 getColor();

    /**
     * Set the color of the resource
     *
     * @param color The color to set
     */
	void setColor(Uint32 color);

    /**
     * Step the resource. This function should update the resource
     * that depend on time. This function is called every iteration.
     *
     * @param SDL_Surface
     *    The SDL_Surface the resource is managed on, convenient when
     *    calling display or hide from the step function
     */
	virtual void step(SDL_Surface *surface) = 0;
};




#endif /* RESOURCE_H_ */
