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

#include "Utilities/SDL_gfxPrimitivesExtra.h"

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

TypedEnergyPoint::TypedEnergyPoint(int id, SDL_Surface *surface) : EnergyPoint(id, surface){
}

TypedEnergyPoint::TypedEnergyPoint(int id, SDL_Surface *surface, int type) : EnergyPoint(id, surface,TypedEnergyPoint::getColor(type)){
    _type = type;
}

TypedEnergyPoint::~TypedEnergyPoint() {
	// TODO Auto-generated destructor stub
}

void TypedEnergyPoint::setType(int type){
	setColor(TypedEnergyPoint::getColor(type));
    _type = type;
}

int TypedEnergyPoint::getType(){
    return _type;
}

void TypedEnergyPoint::hide(SDL_Surface *surface)
{
    if(getVisibility()){
        EnergyPoint::hide(surface);

        // re-display energypoints that we overlapped
        std::vector<ResourceFactory<TypedEnergyPoint>::ResourcePtr> energyPoints = ResourceFactory<TypedEnergyPoint>::getInstance()->getResources();
        for(int i=0;i<energyPoints.size();i++){
            if(energyPoints[i]->getActiveStatus() && energyPoints[i]->getID() != this->getID()&& getEuclidianDistance(energyPoints[i]->getPosition().x, energyPoints[i]->getPosition().y, this->getPosition().x, this->getPosition().y) < (energyPoints[i]->getRadius() + this->getRadius())){
                energyPoints[i]->display(surface);
            }
        }
    }
}

void TypedEnergyPoint::display(SDL_Surface *surface)
{
    if(getVisibility()){
        EnergyPoint::display(surface);

        std::string str = "";
        str += boost::lexical_cast<std::string > (_type);
        RenderTextToSurface(str, getPosition().x - 0.5 * getRadius(), getPosition().y - getRadius(), gBackgroundImage, NULL);
    }
}

Uint32 TypedEnergyPoint::getColor(int type){
	if(type<10 && type>=0){
		return TypedEnergyPoint::colors[type];
	}
	else
		return TypedEnergyPoint::colors[0];
};
