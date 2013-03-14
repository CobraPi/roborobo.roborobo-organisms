/*
 * TypedEnergyPoint.h
 *
 *  Created on: Mar 13, 2013
 *      Author: nico
 */

#ifndef TYPEDENERGYPOINT_H_
#define TYPEDENERGYPOINT_H_

#include "EnergyPoint.h"

class TypedEnergyPoint: public EnergyPoint {
private:
    int _type;
    static Uint32 colors[10];

public:
    TypedEnergyPoint(int id, int type);

	virtual ~TypedEnergyPoint();

    void setType(int type);
    int getType();

    static Uint32 getColor(int type);

	virtual void display();
	virtual void hide();
};

#endif /* TYPEDENERGYPOINT_H_ */
