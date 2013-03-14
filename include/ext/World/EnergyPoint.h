/*
 *  EnergyPoint.h
 *  roborobo
 *
 *  Created by Jean-Marc on 11/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ENERGYPOINT_H
#define ENERGYPOINT_H

#include "RoboroboMain/common.h"
#include "Utilities/Misc.h"
#include "World/Resource.h"

class EnergyPoint : public Resource{
	private :

		static int _nextId;

		int _id;

		double _radius; // radius, in pixels.

		int _energyPointValue; // default: gEnergyPointValue
		bool _energyPointValueIsLocal; // default: false -- WARNING, you have to use get/set method to actually make the energy point behave according to this flag

		bool _fixedLocation; // if false, location changes anytime point is harvested
		int _internLagCounter; // when inactive, count from 0 to respawnLag
		int _respawnLag; // time to respawn - unused for the moment.
		bool _respawnMethodIsLocal; // default: false
		int _energyPointRespawnLagMaxValue; // default: gEnergyPointRespawnLagMaxValue (but not used as long as _respawnMethodIsLocal is false)

		bool _active; // active points can be harvested, inactive points do not exist.
		bool _visible; // display option

		double _key;
		double _initLock;

		int _iterationMax;

        void initialize();
        bool isCollision();

	public :

		EnergyPoint();
		EnergyPoint(int id);
		EnergyPoint(int id,Uint32 color);
		~EnergyPoint();

		double getRadius();
		int getID();

		bool getVisibility();
		void setVisibility(bool visible);

		bool getEnergyPointValueIsLocal();
		void setEnergyPointValueIsLocal( bool __value );

		int getEnergyPointValue();
		void setEnergyPointValue( int __value );

		int getRespawnLag();
		void setRespawnLag( int __lag );

		void setRespawnLagMaxValue( int __lagMaxValue );
		int getRespawnLagMaxValue();

		void setRespawnLagMethodIsLocal( bool __value );
		bool getRespawnLagMethodIsLocal();

		void setActiveStatus( bool __value );
		bool getActiveStatus();

		float getKey();
		void setKey(float key);
		
		void setFixedLocationStatus( bool __value );
		bool getFixedLocationStatus();

		virtual void step();
		virtual void display();
		virtual void hide();
};

#endif
