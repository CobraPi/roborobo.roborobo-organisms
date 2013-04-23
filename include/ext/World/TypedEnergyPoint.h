/*
 * TypedEnergyPoint.h
 *
 *  Created on: Mar 13, 2013
 *      Author: nico
 */

#ifndef TYPEDENERGYPOINT_H_
#define TYPEDENERGYPOINT_H_

#include "World/EnergyPoint.h"
#include "World/ResourceFactory.h"

class TypedEnergyPoint: public EnergyPoint {
private:
    friend class ResourceFactory<TypedEnergyPoint>;
    
    int _type;
    static Uint32 colors[10];

    virtual void display(SDL_Surface *surface);
    virtual void hide(SDL_Surface *surface);
public:
    TypedEnergyPoint(int id, SDL_Surface *surface);
    TypedEnergyPoint(int id, SDL_Surface *surface, int type);
	virtual ~TypedEnergyPoint();

    void setType(int type);
    int getType();

    static Uint32 getColor(int type);
};

#endif /* TYPEDENERGYPOINT_H_ */
