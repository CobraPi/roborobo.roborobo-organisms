//
//  WallAvoidanceBehavior.cpp
//  roborobo
//
//  Created by Berend Weel on 3/14/13.
//
//

#include "WallAvoidanceBehavior.h"

void WallAvoidanceBehavior::step(RobotAgentWorldModel *_wm){
    // a basic obstacle avoidance behavior
    
    double speed =  + 1 - ( (double)gSensorRange - ((_wm->getDefaultSensors()->getSensorDistanceValue(2)+_wm->getDefaultSensors()->getSensorDistanceValue(3))/2) ) / (double)gSensorRange;

    stepSpeed(_wm, speed);
}

void WallAvoidanceBehavior::stepSpeed(RobotAgentWorldModel *_wm, double speed){
    _wm->_desiredTranslationalValue = speed;
    if ( _wm->getDefaultSensors()->getSensorDistanceValue(0) + _wm->getDefaultSensors()->getSensorDistanceValue(1) + _wm->getDefaultSensors()->getSensorDistanceValue(2) < _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) ) {
		_wm->_desiredRotationalVelocity = +5;
    } else if ( _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) < 3*gSensorRange ) {
		_wm->_desiredRotationalVelocity = -5;
    } else if ( _wm->_desiredRotationalVelocity > 0 ) {
		_wm->_desiredRotationalVelocity--;
	} else if ( _wm->_desiredRotationalVelocity < 0) {
        _wm->_desiredRotationalVelocity++;
    } else {
		_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
    }
}

void WallAvoidanceBehavior::stepSpeedDifferential(RobotAgentWorldModel *_wm, double &left, double &right, double speed){
    left = right = speed;
    if ( _wm->getDefaultSensors()->getSensorDistanceValue(0) + _wm->getDefaultSensors()->getSensorDistanceValue(1) + _wm->getDefaultSensors()->getSensorDistanceValue(2) < _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) ) {
		left -= 0.1;
    } else if ( _wm->getDefaultSensors()->getSensorDistanceValue(3) + _wm->getDefaultSensors()->getSensorDistanceValue(4) + _wm->getDefaultSensors()->getSensorDistanceValue(5) < 3*gSensorRange ) {
		right -= 0.1;
    } else if ( _wm->_desiredRotationalVelocity > 0 ) {
		_wm->_desiredRotationalVelocity--;
	} else if ( _wm->_desiredRotationalVelocity < 0) {
        _wm->_desiredRotationalVelocity++;
    } else {
        if((double)(rand()%10)/10.0 < 0.5){
            left += (double)(rand()%10)/10.0*0.02;
        }else{
            right += (double)(rand()%10)/10.0*0.02;
        }
    }
}