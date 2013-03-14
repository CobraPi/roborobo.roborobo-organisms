/*
 *  DifferentialControlArchitecture.cpp
 *  roborobo
 *
 *  Created by Robert-Jan Huijsman on 01-09-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DifferentialControlArchitecture.h"

const double DifferentialControlArchitecture::maxSpeed = (0.05 * PIXELSPERMETER) / TICKSPERSECOND; // 0.05 m/s, converted to pixels/tick

DifferentialControlArchitecture::DifferentialControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
	// nothing to do
}

DifferentialControlArchitecture::~DifferentialControlArchitecture()
{
	// nothing to do.
}

void DifferentialControlArchitecture::setSpeed(double left, double right) {
	if (left > 1) {
		left = 1;
	}
	if (left < -1) {
		left = -1;
	}
	if (right > 1) {
		right = 1;
	}
	if (right < -1) {
		right = -1;
	}
	
	left *= maxSpeed;
	right *= maxSpeed;
		
	/* conversion of differential drive values to
	 forward and turn speeds from
	 http://www.physicsforums.com/showthread.php?t=263149 */
	
	/* wheel separation of the simplebot - make sure this suits the robot */
	double separation = 0.1; // 10 cm
	double newspeed, newturnrate;
	
	// which is the outer wheel and which is the inner?
	if (left > right) { // left is outer wheel
		// newspeed is average between inner and outer wheel speed
		newspeed = right + (left - right)/2;
		newturnrate = (right - left) / separation;
	}
	else if (right > left) { //right is outer wheel
		// newspeed is average between inner and outer wheel speed
		newspeed = left + (right - left)/2;
		newturnrate = (right - left) / separation;
	}
	else {
		newspeed = left; //could also be right
		newturnrate = 0;
	}
	
	/* At this point, newspeed is in m/s, newturnrate is in radians/s. This needs to be converted to
		pixels per tick and degrees per tick. For this we need two pieces of info:
		- the number of ticks per second
		- the number of pixels per meter
	   These have been defined in the header file; read about their approximate values there.
	  */
	
	// convert from m/s to pixels/tick
	newspeed = (newspeed * PIXELSPERMETER) / TICKSPERSECOND;
	
	// convert from radians/s to degrees/tick
	newturnrate = (newturnrate * 180) / TICKSPERSECOND;
	
	if(newturnrate != 0){
		_wm->_desiredRotationalVelocity = -newturnrate; // *180 because newTurnRate is in radials
	}else{
		_wm->_desiredRotationalVelocity = 0;
	}
	_wm->_desiredTranslationalValue = newspeed;
	//cout << "Set speed (" << left << ", " << right << "): " << newspeed << ", rotation: " << _wm->_desiredRotationalVelocity << endl;

}

vector<double>* DifferentialControlArchitecture::getSensorValues() {
	vector<double>* sensors = new vector<double>(_wm->getDefaultSensors()->getSensorCount());
	for (int i = 0; i < _wm->getDefaultSensors()->getSensorCount(); i++) {
		sensors->push_back(_wm->getDefaultSensors()->getSensorDistanceValue(i) / _wm->getDefaultSensors()->getSensorMaximumDistanceValue(i));
	}
         return sensors;
}
