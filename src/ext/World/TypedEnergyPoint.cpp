/*
 * TypedEnergyPoint.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: nico
 */

#include "World/TypedEnergyPoint.h"
#include "RoboroboMain/roborobo.h"

#include "Dynamic/include/DynamicSharedData.h"
#include "SDL_collide.h"

#include <cstring>
#include <boost/lexical_cast.hpp>

extern int RenderTextToSurface(std::string Text, int x, int y, SDL_Surface *Dest, SDL_Color *TXT_Color);

Uint32 TypedEnergyPoint::colors[10] = {
    0x01ff00ff, // Purple
    0x02aa00ff,
    0x038800ff,
    0x0400ffff,
    0x0500aaff,
    0x060088ff,
    0x07ff00ff,
    0x08aa00ff,
    0x098800ff,
    0x0affffff};

TypedEnergyPoint::TypedEnergyPoint(int id, int type):EnergyPoint(id,TypedEnergyPoint::getColor(type)){
    _type = type;
}

TypedEnergyPoint::~TypedEnergyPoint() {
	// TODO Auto-generated destructor stub
}

void TypedEnergyPoint::setType(int type){
	setColor(TypedEnergyPoint::getColor(type));
    _type = type;
    if(getVisibility()){
    	hide();
        display();
    }
}

int TypedEnergyPoint::getType(){
    return _type;
}

void TypedEnergyPoint::hide()
{
	EnergyPoint::hide();

    // re-display energypoints that we overlapped
    for(int i=0;i<gTypedEnergyPoints.size();i++){
        for(int j=0;j<gTypedEnergyPoints[i].size();j++){
            if(gTypedEnergyPoints[i][j].getActiveStatus() && gTypedEnergyPoints[i][j].getID() != this->getID()&& getEuclidianDistance(gTypedEnergyPoints[i][j].getPosition().x, gTypedEnergyPoints[i][j].getPosition().y, this->getPosition().x, this->getPosition().y) < (gTypedEnergyPoints[i][j].getRadius() + this->getRadius())){
                gTypedEnergyPoints[i][j].display();
            }
        }
    }
}

void TypedEnergyPoint::display()
{
	EnergyPoint::display();

	std::string str = "";
	str += boost::lexical_cast<std::string > (_type);
	RenderTextToSurface(str, getPosition().x - 0.5* getRadius(), getPosition().y - getRadius(), gBackgroundImage, NULL);
}

Uint32 TypedEnergyPoint::getColor(int type){
	if(type<10 && type>=0){
		return TypedEnergyPoint::colors[type];
	}
	else
		return TypedEnergyPoint::colors[0];
};
