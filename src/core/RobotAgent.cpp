/**
 *  RobotAgent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 *  Changed by Job Bakker on 07/03/2013
 *  - changed the sensor handling, now using the Sensors class.
 *  - added DoxyGen style-like documentation
 *
 */

#include <cmath>
#include <cstring>
#include <boost/lexical_cast.hpp>

#include "Config/GlobalConfigurationLoader.h"

#include "Agents/RobotAgent.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "BehaviorControlArchitectures/EnergyPerceptronControlArchitecture.h"
#include "SDL_collide.h"

/**
 * Create a new RobotAgent for the World __world. The RobotAgent class is called
 * in the roborobo (main) class.
 *
 * @param __world
 * 		The World to create the RobotAgent in.
 */
RobotAgent::RobotAgent(World *__world) {
	_wm = gConfigurationLoader->make_RobotAgentWorldModel(); // TODO: externalize object referenced to create the new instance
	_registered = false;

	_wm->_world = __world;

	_wm->_agentId = gAgentCounter;
	gAgentCounter++;

	_organism = NULL;
	connected = new std::vector<RobotAgentPtr>();
	//Process agent specification (ie. IR/US/laser sensors)

	// create dynamic array
	// parse image and add (sensor.x/y, orientation) or (sensorStart.x/y, sensorEnd.x/y)
	_wm->initSensors();
	this->_agentMask = gAgentMaskImage;
	
	_agentObserver = gConfigurationLoader->make_AgentObserver(_wm); // TODO: externalize
	_behavior = gConfigurationLoader->make_BehaviorControlArchitecture(_wm); // TODO: externalize

	reset();

}

RobotAgent::~RobotAgent() {
	delete _agentObserver;
    _agentObserver = NULL;
    connected->clear();
	delete connected;
    connected = NULL;
	delete _wm;
    _wm = NULL;
	delete _behavior;
    _behavior = NULL;
    
    _organism = NULL;
}

void RobotAgent::reset() {
	//Initialize general
	_iterations = 0;
	_wm->_isAlive = true;


	/**/
	int x = 0, y = 0;
	std::string s = "";
	s += "agent[";
	std::stringstream out;
	out << _wm->_agentId;
	s += out.str();
	s += "].x";

	bool randomStart = false;
	if (gProperties.hasProperty(s))
		convertFromString<int>(x, gProperties.getProperty(s), std::dec);
	else {
		if (gVerbose) {
			std::cout << "[warning] Initial X coordinate for agent #" << _wm->_agentId << " not found. Random pick ([!] reset may alter environment, check with x-ray mode [!])." << std::endl;
		}
		randomStart = true;
		x = (int) gAgentWidth + (ranf() * (double) (gSpawnWidth - gAgentWidth));
	}

	s = "agent[";
	s += out.str();
	s += "].y";
	if (gProperties.hasProperty(s))
		convertFromString<int>(y, gProperties.getProperty(s), std::dec);
	else {
		if (gVerbose) {
			std::cout << "[warning] Initial Y coordinate for agent #" << _wm->_agentId << " not found. Random pick ([!] reset may alter environment, check with x-ray mode [!])." << std::endl;
		}
		randomStart = true;
		y = (int) gAgentHeight + (ranf() * (double) (gSpawnHeight - gAgentHeight));
	}

	setCoordReal(x, y);
	setCoord(x, y);
	
	while (randomStart && isCollision()) {
		//std::cout << "Collision detected on " << x << ", " << y << " rerolling" << std::endl;
		x = (int) 20 + (ranf() * (double) (gSpawnWidth));
		y = (int) 20 + (ranf() * (double) (gSpawnHeight));
		setCoordReal(x, y);
		setCoord(x, y);
	}

	// register agent so others don't start on top of it
	registerAgent();

	//_wm->_xReal = x;
	//_wm->_yReal = y;
	/**/

	//Initialize coordinate and displacement
	//_wm->_xReal=gAgentXStart;
	//_wm->_yReal=gAgentYStart;
	_xDelta = 0;
	_yDelta = 0;

	//Initialize internal variables
	_xDeltaReal = 0; //gAgentXStart;
	_yDeltaReal = 0; //gAgentYStart;


	s = "agent[";
	s += out.str();
	s += "].orientation";
	if (gProperties.hasProperty(s))
		convertFromString<double>(_wm->_agentAbsoluteOrientation, gProperties.getProperty(s), std::dec);
	else {
		if (gVerbose) {
			std::cout << "[warning] Initial orientation  for agent #" << _wm->_agentId << " not found. Random pick." << std::endl;
		}
		_wm->_agentAbsoluteOrientation = ranf() * 360. - 180.;
	}

	_wm->_agentAbsoluteLinearSpeed = 0;

	_wm->_desiredRotationalVelocity = 0;
	_wm->_desiredTranslationalValue = 0;

	_wm->_maxRotationalDeltaValue = gMaxRotationalSpeed; // ie. change will be instantenous
	_wm->_maxTranslationalDeltaValue = gMaxTranslationalDeltaValue; //gMaxTranslationalSpeed ; // idem.

	_wm->_actualTranslationalValue = 0;
	_wm->_actualRotationalVelocity = 0;
    
    _wm->resetSensors();

	// Initialize agent observer and Behavior Control Architecture

	_agentObserver->reset();
	_behavior->reset();

}

void RobotAgent::callObserver() {
	_agentObserver->step();
}

void RobotAgent::stepBehavior() {
	// update status
	_iterations++;

	if (_wm->_isAlive) // note: reviving a robot is performed in stepGenetics()
	{
		// compute commands
		_behavior->step();
	}
	
	// TODO: Fix leak
	_wm->updateSensors();

	displayInfo();
}

void RobotAgent::stepBehavior(Uint8* __keyboardStates) {
	_iterations++;

	// compute commands
	behaviorUpdate_remoteController(__keyboardStates);

	displayInfo();
}

void RobotAgent::displayInfo() {
	if (gVerbose && gDisplayMode <= 1){
		if (gInspectAgent && gAgentIndexFocus == _wm->_agentId) {
			std::cout << "Robot #" << _wm->_agentId << " : ( ";
            _wm->displaySensorInformation();
			std::cout << " ) => ( " << _wm->_desiredTranslationalValue << " ; " << _wm->_desiredRotationalVelocity << " )." << std::endl;
		}
	}
}

void RobotAgent::behaviorUpdate_remoteController(Uint8* __keyboardStates) // user controlled robot
{
	if (gAgentIndexFocus != _wm->_agentId)
		return;

	// key pressed
	gKeyUp = __keyboardStates[ SDLK_UP ];
	gKeyDown = __keyboardStates[ SDLK_DOWN ];
	gKeyLeft = __keyboardStates[ SDLK_LEFT ];
	gKeyRight = __keyboardStates[ SDLK_RIGHT ];

	// update speed

	if (gKeyUp)
		_wm->_desiredTranslationalValue += 1; // pixels
	if (gKeyDown)
		_wm->_desiredTranslationalValue += -1;

	if (_wm->_desiredTranslationalValue > gMaxTranslationalSpeed)
		_wm->_desiredTranslationalValue = gMaxTranslationalSpeed;
	else
		if (_wm->_desiredTranslationalValue < -gMaxTranslationalSpeed)
		_wm->_desiredTranslationalValue = -gMaxTranslationalSpeed;

	if (gKeyLeft)
		_wm->_desiredRotationalVelocity = -10; // degrees
	if (gKeyRight)
		_wm->_desiredRotationalVelocity = 10;

	if (!gKeyUp && !gKeyDown)
		_wm->_desiredTranslationalValue = 0;

	if (!gKeyRight && !gKeyLeft) {
		if (_wm->_desiredRotationalVelocity > 0)
			_wm->_desiredRotationalVelocity--;
		else
			if (_wm->_desiredRotationalVelocity < 0)
			_wm->_desiredRotationalVelocity++;
	}

	if (__keyboardStates[SDLK_o]) {
		_wm->_desiredTranslationalValue = 0;
		_wm->_desiredRotationalVelocity = 0;
	}
}

/*
	Compute actual Translational value and Rotational velocity that can be applied given the agent/robot dynamics.
	By default, this means: apply min/max values bounds, apply threshold on maximum delta values wrt. current values.
	By extension, this means: also apply some specific environment impact on the agent/robot dynamics (e.g. slow down due to sticky terrain, etc.). Details:
	1. convert desired translation/rotation to possible translation/rotation values
	2. apply environment constraints (call external method)
	3. translate into internal delta position update (ie. 2d x/y delta values)
 */
void RobotAgent::applyDynamics() {

	this->_lastAgentAbsoluteOrientation = _wm->_agentAbsoluteOrientation; // in case of collision, original orientation should be restored.

	// * translation
	// assume controller gives desired speed (the following models a feedback system (fr.: systeme asservi))

	_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed;

	if (_wm->_desiredTranslationalValue != _wm->_agentAbsoluteLinearSpeed) {
		double diff = fabs(_wm->_desiredTranslationalValue - _wm->_agentAbsoluteLinearSpeed); // Note: using abs(...) would truncate to integer. Use fabs instead. (minor bug solved as of 2010 06 18)

		if (_wm->_desiredTranslationalValue > _wm->_agentAbsoluteLinearSpeed) {
			if (diff > _wm->_maxTranslationalDeltaValue)
				_wm->_actualTranslationalValue += _wm->_maxTranslationalDeltaValue;
			else
				_wm->_actualTranslationalValue += diff;
		} else // strictly below
		{
			if (diff > _wm->_maxTranslationalDeltaValue)
				_wm->_actualTranslationalValue -= _wm->_maxTranslationalDeltaValue;
			else
				_wm->_actualTranslationalValue -= diff;
		}

		// upper and lower bound for translation speed
		if (_wm->_actualTranslationalValue > gMaxTranslationalSpeed)
			_wm->_actualTranslationalValue = gMaxTranslationalSpeed;
		else
			if (_wm->_actualTranslationalValue < -gMaxTranslationalSpeed)
			_wm->_actualTranslationalValue = -gMaxTranslationalSpeed;
	}

	// * rotation
	// assume controller gives angular velocity.

	if (_wm->_desiredRotationalVelocity > 0) {
		if (_wm->_desiredRotationalVelocity <= _wm->_maxRotationalDeltaValue)
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		else
			_wm->_actualRotationalVelocity = _wm->_maxRotationalDeltaValue;
	} else
		if (_wm->_desiredRotationalVelocity < 0) {
		if (_wm->_desiredRotationalVelocity >= -_wm->_maxRotationalDeltaValue)
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		else
			_wm->_actualRotationalVelocity = -_wm->_maxRotationalDeltaValue;
	}

	// * compute true displacement (actual (thought to be true) and true (really effected) rotation and translation,

	// update both actual trans/rot values and internal data (see method definition for detailed comments)
	// after this call, wm->_agentAbsoluteLinearSpeed AND _wm->_agentAbsoluteOrientation are updated (internal data)
	gConfigurationLoader->make_RobotAgentWorldInterface()->applyInternalDynamics(_wm);


	// note: the next locomotion-related method called will be the "move" method. It handles collision.
	// In case of collision, actual translation/rotation variables will be updated accordingly
	// Keep in mind that roborobo philosophy assumes pixel-based level of details for collision.
}

/**
 * update the agent position in the environment. Apply simple physics (ie. obstacle collision detection and consequences).
 * if collision, translation and rotation speed are set to zero.
 * note: __recursiveIt is currently unused (maybe used for debug purpose, eg. checking for infinite loop.)
 */
void RobotAgent::move(int __recursiveIt) // the interface btw agent and world -- in more complex envt, this should be handled by the "world".
{
	// apply world dynamics onto this agent

	// compute real valued delta (convert to x/y delta coordinates)
	applyDynamics();

	// save position
    oldPosition = _wm->getPosition();
//	this->xReal_old = _wm->_xReal; // backup old position in case of collision
//	this->yReal_old = _wm->_yReal;


	/*for (int i = 0; i < gNbOfAgents; i++) {
		gWorld->getAgent(i)->unregisterAgent();
	}*/

	// update x/y position

    Point2d newPosition = oldPosition;
	_xDeltaReal = _wm->_agentAbsoluteLinearSpeed * cos(_wm->_agentAbsoluteOrientation * M_PI / 180);
	_yDeltaReal = _wm->_agentAbsoluteLinearSpeed * sin(_wm->_agentAbsoluteOrientation * M_PI / 180);

	newPosition.x += _xDeltaReal;
    newPosition.y += _yDeltaReal; // TODO: round is for positive values... (ok in this case however as 0,0 is up-left)

    _wm->setPosition(newPosition);
	setCoord((int) newPosition.x + 0.5, (int) newPosition.y + 0.5);

	// * collision with (image) border of the environment - position at border, then bounce

	//Move the collision boxes
	//shift_boxes(); // NOT IMPLEMENTED

	if (isCollision()) {
        _wm->_desiredTranslationalValue = 0;
        
		if (_wm->_agentAbsoluteLinearSpeed >= 1.0) {
			_wm->_agentAbsoluteLinearSpeed--;
		} else {
			if (_wm->_agentAbsoluteLinearSpeed <= -1.0)
				_wm->_agentAbsoluteLinearSpeed++;
		}

		if ((_wm->_agentAbsoluteLinearSpeed > -1.0) && (_wm->_agentAbsoluteLinearSpeed < 1.0)) {
			_wm->_agentAbsoluteLinearSpeed = 0; // roborobo assumes pixel-based resolution for solving collision. Ie. ignore sub-pixel translation values. (see _README.TXT for details)
		}
		//_wm->_agentAbsoluteLinearSpeed = 0;

        _wm->setPosition(oldPosition);
		if (!std::isnan(_wm->_agentAbsoluteLinearSpeed) && _wm->_agentAbsoluteLinearSpeed != 0) // if zero: move is canceled
		{
			__recursiveIt++;
			move(__recursiveIt);
		} else {
			// special case: agent cannot not move at all - restore original coordinate (remember, _x/yReal are global and modified during recursive call).
            _wm->setPosition(oldPosition);
			setCoord((int) oldPosition.x + 0.5, (int) oldPosition.y + 0.5);
		}

		// update world model variables and internal data

		_wm->_agentAbsoluteLinearSpeed = 0;

		if (gLocomotionMode == 1) // consider obstacle friction or not for rotation?
		{
			_wm->_agentAbsoluteOrientation = _lastAgentAbsoluteOrientation;
			_wm->_actualRotationalVelocity = 0;
		} else
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;

		//_wm->_desiredTranslationalValue = _wm->_desiredRotationalVelocity = 0;
		_wm->_actualTranslationalValue = 0;

	} else {
		// actual rotational and translational values matches desired values
		_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed; // (!) _wm->_desiredTranslationalValue is different as the "desired" translation speed may not be reached due to physical actuator limitations
	}

	/*for (int i = 0; i < gNbOfAgents; i++) {
		if (gWorld->getAgent(i)->_wm->_agentId != this->_wm->_agentId) {
			gWorld->getAgent(i)->registerAgent();
		}
	}*/

	/*
	if ( collision ) // bounce (TODO: position at collision point)
	{
		_x=x_old;
		_y=y_old;
		_xDelta = 0;
		_yDelta = 0;
		_xReal=xReal_old;
		_yReal=yReal_old;
		_xDeltaReal=_yDeltaReal=0;
	}
	 */

	// update sensors
    _wm->updateSensors();
}

bool RobotAgent::isCollision() {
	// check collision with borders and environment objects.
	if (
		(_x < 0) || (_x + gAgentWidth >= gAreaWidth) ||
		(_y < 0) || (_y + gAgentHeight >= gAreaHeight)
		) {
		// * collision with border
		return true;
	} else {
        // * environment objects
		for ( int i = 0 ; i != gAgentWidth ; i++ ) {
			for ( int j = 0 ; j != gAgentHeight ; j++ )
			{
				if ( getPixel32( gAgentMaskImage , i , j) != SDL_MapRGB( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF ) ) // opt: bounding box instead of pixel-to-pixel test.
				{
                    // USELESS
				    if (
						( _x+i < 0 ) || ( _x + i  >= gAreaWidth ) ||
						( _y+j < 0 ) || ( _y + i  >= gAreaHeight )
						)
					{
						return true;
					}
                    
					Uint32 pixel = getPixel32( gEnvironmentImage , _x+i , _y+j);
					if (  pixel != SDL_MapRGB( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF ) )
					{
						return true;
					}
				}
			}
        }
	}
	return false;
}

/**
 * Display agent on screen. Add information caption if needed.
 *
 * (render mode only)
 */
void RobotAgent::show() // display on screen
{
	//Show the dot

	if (gNiceRendering) {
			apply_surface(_x - gCamera.x, _y - gCamera.y, this->_agentMask, gScreen);
	}

	if(gRenderRobotId){
		std::string str = "";
		str += boost::lexical_cast<std::string > (this->_wm->_agentId);
		RenderTextToSurface(str, _x + (gAgentWidth / 4) - gCamera.x, _y - gCamera.y, gScreen, NULL);
	}

    if(gShowAgentEnergyPoints){
		int value = this->getWorldModel()->getEnergyLevel();

        SDL_Color TXT_Color;
		TXT_Color.g = (value < 255 ? value : 255);
		TXT_Color.r = 255 - TXT_Color.g;
		TXT_Color.b = 0x00;

		RenderTextToSurface(boost::lexical_cast<std::string>(value), _x + (gAgentWidth / 4)  + 10 - gCamera.x, _y + 10 - gCamera.y, gScreen, &TXT_Color);
    }

	if (getWorldModel()->getRobotLED_status() == true) {
		int dx = 1;
		int dy = 1;
		int xcenter = (int) _wm->getPosition().x + 0.5;
		int ycenter = (int) _wm->getPosition().y + 0.5;
		int r = getWorldModel()->getRobotLED_redValue();
		int g = getWorldModel()->getRobotLED_greenValue();
		int b = getWorldModel()->getRobotLED_blueValue();

		for (int xTmp = xcenter - dx; xTmp != xcenter + dx + 1; xTmp++)
			for (int yTmp = ycenter - dy - 1; yTmp != ycenter + dy; yTmp++) {
				putPixel32secure(gScreen, xTmp - gCamera.x, yTmp + dy - gCamera.y, SDL_MapRGB(gScreen->format, r, g, b));
			}
	}

	/**
	 * Display agent on screen. Add information caption if needed.
	 */
	if (_wm->_agentId == gAgentIndexFocus && gUserCommandMode) // && _iterations%10 < 5)
	{
		int dx = 10;
		int dy = 10;
		int xcenter = (int) _wm->getPosition().x + 0.5;
		int ycenter = (int) _wm->getPosition().y + 0.5;
		int r = 255.0 * ranf();
		int g = 255.0 * ranf();
		int b = 255.0 * ranf();

		for (int xTmp = xcenter - dx; xTmp != xcenter + dx + 1; xTmp++) {
			putPixel32secure(gScreen, xTmp - gCamera.x, ycenter - dy - gCamera.y, SDL_MapRGB(gScreen->format, r, g, b));
			putPixel32secure(gScreen, xTmp - gCamera.x, ycenter + dy - gCamera.y, SDL_MapRGB(gScreen->format, r, g, b));
		}

		for (int yTmp = ycenter - dy; yTmp != ycenter + dy + 1; yTmp++) {
			putPixel32secure(gScreen, xcenter - dx - gCamera.x, yTmp - gCamera.y, SDL_MapRGB(gScreen->format, r, g, b));
			putPixel32secure(gScreen, xcenter + dx - gCamera.x, yTmp - gCamera.y, SDL_MapRGB(gScreen->format, r, g, b));
		}

		//			for ( int xTmp = (int)_wm->_xReal - dx ; xTmp != (int)_wm->_xReal + dx + 1 ; xTmp++ )
		//				for ( int yTmp = (int)_wm->_yReal - dy ; yTmp != (int)_wm->_yReal + dy + 1 ; yTmp++ )
		//						putPixel32secure( gScreen, xTmp - gCamera.x, yTmp - gCamera.y , SDL_MapRGB( gScreen->format, 0xFF, 0x00, ranf() ) );
	}


	if (gDisplaySensors == true) {
		// * show orientation

        Point2d position = _wm->getPosition();
		int xOrientationMarker = (int) (position.x + 0.5) + gAgentWidth / 2 * cos(_wm->_agentAbsoluteOrientation * M_PI / 180);
		int yOrientationMarker = (int) (position.y + 0.5) + gAgentWidth / 2 * sin(_wm->_agentAbsoluteOrientation * M_PI / 180);

		if (_wm->_agentId == gAgentIndexFocus && gUserCommandMode) {
			int g, b;
			g = b = (32 * _iterations % 256) > 128 ? 0 : 255;
			for (int xTmp = -2; xTmp != 3; xTmp++)
				for (int yTmp = -2; yTmp != 3; yTmp++)
					putPixel32secure(gScreen, xOrientationMarker - gCamera.x + xTmp, yOrientationMarker - gCamera.y + yTmp, SDL_MapRGB(gScreen->format, 0x00, b, g));
		} else {
			for (int xTmp = -2; xTmp != 3; xTmp++)
				for (int yTmp = -2; yTmp != 3; yTmp++)
					putPixel32secure(gScreen, xOrientationMarker - gCamera.x + xTmp, yOrientationMarker - gCamera.y + yTmp, SDL_MapRGB(gScreen->format, 0xFF, 0x00, 0x00));
		}

		// * show sensors
		_wm->displaySensors();
	}


	// caption for storyzones

	if (gDisplayZoneCaption) {
		Uint32 pixel = getPixel32(gZoneImage, _x + gAgentWidth / 2, _y + gAgentHeight / 2);
		if (pixel != SDL_MapRGB(gZoneImage->format, 0x00, 0x00, 0x00)) // check if there's a "story" to display
		{
			// extract story index (if any)
			Uint8 r, g, b;
			SDL_GetRGB(pixel, gZoneImage->format, &r, &g, &b);
			int storyId = r; // assume the red component holds the story index.

			if (storyId >= 0 && storyId < 256 && gZoneCaptionImage[storyId] != NULL && gZoneStatus[storyId]) // security check: story exists?
			{
				// display story caption

				//set caption position
				int xCaption = 0, yCaption = 0;
				if (_x < gAreaWidth / 2)
					xCaption = _x - gCamera.x + 40;
				else
					xCaption = _x - gCamera.x - gZoneCaptionImage[storyId]->w - 40;
				if (_y < gAreaHeight / 2)
					yCaption = _y - gCamera.y + 40;
				else
					yCaption = _y - gCamera.y - gZoneCaptionImage[storyId]->h - 40;

				//display caption
				apply_surface(xCaption, yCaption, gZoneCaptionImage[storyId], gScreen);

				// update story flags (if needed)
				if (storyId >= 100 && storyId < 200 && storyId % 10 == 0) // key story, btw 100 and 199: activate story flag of the 9 next sub-stories
				{
					for (int i = 1; i != 10; i++)
						gZoneStatus[storyId + i] = true;
				} else
					if (storyId >= 200 && storyId < 256 && storyId % 10 != 9) // sub-story, btw 200 and 299: activate the next story flag (if not last)
				{
					gZoneStatus[storyId + 1] = true;
				}
			}
		}
	}
}

void RobotAgent::registerAgent() {
	if (gAgentRegistration && !this->_registered) {
        // [!n] CONVENTION: _agentId+1000 to avoid confusion with empty space (0: empty ; 1: static object; 2-99: reserved for other static objects)
		register_surface(_x, _y, getAgentMask(), gEnvironmentImage, _wm->_agentId + 1000);
		this->_registered = true;
	}
}

void RobotAgent::unregisterAgent() {
	if (gAgentRegistration && this->_registered) {
		clean_surface(_x, _y, getAgentMask(), gEnvironmentImage);
		this->_registered = false;
	}
}

bool RobotAgent::isRegistered(){
	return _registered;
}

void RobotAgent::setAgentMask(SDL_Surface* mask){
    unregisterAgent();
    this->_agentMask = mask;
    registerAgent();
}

SDL_Surface* RobotAgent::getAgentMask(){
    return _agentMask;
}

void RobotAgent::setCoordReal(int __x, int __y) // agent is centered on point
{
    Point2d pos(__x,__y);
    _wm->setPosition(pos);
}

AgentObserver* RobotAgent::getObserver() {
	return _agentObserver;
}

BehaviorControlArchitecture* RobotAgent::getBehavior() {
	return _behavior;
}

RobotAgentWorldModel* RobotAgent::getWorldModel() {
	return _wm;
}

std::vector<int> RobotAgent::getNeighbors() {
	std::vector<int> neighborsList;

	if (gRadioNetwork) {
		int id = _wm->_agentId;

		// internal cuisine: for all existing agents, check if in the vicinity. Yes? add to neighbors list.
		for (int id_target = 0; id_target != gAgentCounter; id_target++) {
			if (gRadioNetworkArray[id][id_target] > 0) // note that the diagonal is always zero (ie. self-connection is not considered). check anyway (avoid an extra if statement).
				neighborsList.push_back(id_target);
		}
	} else
		if (gVerbose)
		std::cout << "[WARNING] RobotAgent::getNeighbors() is called but radio network is *inactive*! (always return empty list)" << std::endl;


	return neighborsList;
}

std::vector<RobotAgentPtr> RobotAgent::getNearRobots(){
    return _wm->getDefaultSensors()->getNearRobots(this);
}

// ---- ---- ---- ---- ----

/**
 * This function sets up connections between robots who are close enough to
 * each other and have their connection ring set to 1.
 */
void RobotAgent::setUpConnections() {
	if (!gUseOrganisms) return;
    
	int nAgents = _wm->_world->getNbOfAgent();
	int id = _wm->_agentId;

	if (this->isPartOfOrganism()) {
		if (this->getConnectToOthers() == Agent::NEGATIVE) {
			this->letGoOfOrganism();
		}/* else if (this->getConnectToOthers() == Agent::NEUTRAL) {
			std::vector<RobotAgentPtr>::iterator it;
			for (it = this->connected->begin(); it != this->connected->end(); it++) {
				RobotAgentPtr other = (*it);
				if (other->getConnectToOthers() == Agent::NEUTRAL) {
					it = this->connected->erase(it);
					other->removeNeighbour(gWorld->getAgent(this->_wm->_agentId));
					break;
				}
			}
		}*/
		if (this->connected->empty()) {
			this->letGoOfOrganism();
		}
	}
	for (int i = 0; i < nAgents; i++) {
		RobotAgentPtr other = _wm->_world->getAgent(i);
		RobotAgentWorldModel *otherWM = (RobotAgentWorldModel*) other->getWorldModel();
		int otherId = otherWM->_agentId;

		if (otherId != id) {
			// If they're out of bounds, ignore.
			// This happens at the start of the simulation sometimes and can lead
			// to false positives
			if (this->isOutOfBounds() || other->isOutOfBounds()) {
				return;
			}

            Point2d position = _wm->getPosition();
            Point2d positionOther = otherWM->getPosition();

			// Are they within range?
			if (SDL_CollideBoundingCircle(gAgentMaskImage, position.x, position.y, gAgentMaskImage, positionOther.x, positionOther.y, gConnectionGap)) {
				// connect to other robot
                
                // ---- Mark: but only connect in case you are willing to do so.
                
				if ((this->getConnectToOthers() == Agent::POSITIVE) && (other->getConnectToOthers() == Agent::POSITIVE)) {
					// || other->getConnectToOthers() == Agent::NEUTRAL)) {
					this->connectToRobot(other);
				}
//				else if (this->getConnectToOthers() == Agent::NEUTRAL && other->getConnectToOthers() == Agent::POSITIVE) {
//					this->connectToRobot(other);
//				}
			}
		}
	}
}

void RobotAgent::connectToRobot(RobotAgentPtr other) {
	if (!gUseOrganisms) return;

	if (this->isPartOfOrganism() && other->isPartOfOrganism()
		&& this->getOrganism()->getId() == other->getOrganism()->getId()) {
		// the same organism, do nothing.
	} else if (other->isPartOfOrganism() && this->isPartOfOrganism()
		&& other->getOrganism()->getId() != this->getOrganism()->getId()) {
		// two different organisms
		Organism* o1 = this->getOrganism();
		Organism* o2 = other->getOrganism();

		this->addNeighbour(other);
		other->addNeighbour(gWorld->getAgent(this->_wm->_agentId));

		// merge smaller into larger
		if (o1->size() >= o2->size()) {
			gLogFile << "Merging organism " << o2->getId() << " into " << o1->getId() << std::endl;

			//other->clickOnto(this, o2);

			o2->mergeInto(o1);
			o2->setInactive();
			//Organism::remove(o2);
			//delete o2;
		} else {
			gLogFile << "Merging organism " << o1->getId() << " into " << o2->getId() << std::endl;

			//this->clickOnto(other, o1);

			o1->mergeInto(o2);
			o1->setInactive();
			
			//Organism::remove(o1);
			//delete o1;
		}
	} else if (other->isPartOfOrganism()) {
		Organism* organism = other->getOrganism();
		organism->addRobot(gWorld->getAgent(this->_wm->_agentId));
		this->setOrganism(organism);

		this->addNeighbour(other);
		other->addNeighbour(gWorld->getAgent(this->_wm->_agentId));

		//this->clickOnto(other);
		gLogFile << "Adding agent " << this->_wm->_agentId << " to organism: " << organism->getId() << std::endl;
	} else if (this->isPartOfOrganism()) {
		Organism* organism = this->getOrganism();
		organism->addRobot(other);
		other->setOrganism(organism);

		this->addNeighbour(other);
		other->addNeighbour(gWorld->getAgent(this->_wm->_agentId));

		//other->clickOnto(this);
		gLogFile << "Adding agent " << other->_wm->_agentId << " to organism: " << organism->getId() << std::endl;
	} else {
		Organism* organism = new Organism();
		gLogFile << "Creating new organism: " << organism->getId() << " robots: " << this->_wm->_agentId << ", " << other->_wm->_agentId << std::endl;
		organism->addRobot(gWorld->getAgent(this->_wm->_agentId));
		organism->addRobot(other);

		this->setOrganism(organism);
		other->setOrganism(organism);

		this->addNeighbour(other);
		other->addNeighbour(gWorld->getAgent(this->_wm->_agentId));

		Organism::add(organism);

		//this->clickTogether(other);
	}
}

void RobotAgent::addNeighbour(RobotAgentPtr neighbour) {
	this->connected->push_back(RobotAgentPtr(neighbour));
}

void RobotAgent::removeNeighbour(RobotAgentPtr neighbour) {
	std::vector<RobotAgentPtr>::iterator it;
	for (it = this->connected->begin(); it != this->connected->end(); it++) {
		if ((*it)->_wm->_agentId == neighbour->_wm->_agentId) {
			this->connected->erase(it);
			return;
		}
	}
}

std::vector<RobotAgentPtr> *RobotAgent::getConnected() {
	return this->connected;
}

bool RobotAgent::isOutOfBounds() {
	//If the dot went too far to the left or right
	if ((this->_x < 0) || (this->_x + gAgentWidth > gAreaWidth)) {
		return true;
	}
	//If the dot went too far up or down
	if ((this->_y < 0) || (this->_y + gAgentHeight > gAreaHeight)) {
		return true;
	}
	return false;
}

void RobotAgent::letGoOfOrganism() {
	if (!gUseOrganisms) return;

	if (this->isPartOfOrganism()) {
		gLogFile << "Agent: " << this->_wm->_agentId << " letting go of organism: " << this->getOrganism()->getId() << std::endl;
		this->getOrganism()->removeRobot(gWorld->getAgent(this->_wm->_agentId));

		std::vector<RobotAgentPtr>::iterator it;
		for (it = this->connected->begin(); it != this->connected->end(); it++) {
			(*it)->removeNeighbour(gWorld->getAgent(this->_wm->_agentId));
		}
		this->connected->clear();
	}
}

void RobotAgent::setOrganism(Organism* __organism) {
	if (!gUseOrganisms) return;

	_organism = __organism;
}

Organism* RobotAgent::getOrganism() {
	return _organism;
}

bool RobotAgent::isPartOfOrganism() {
	return _organism != NULL;
}

/**
 * Check whether the other robot is in the same organism
 * @param other
 * @return
 */
bool RobotAgent::isPartOfSameOrganism(RobotAgentPtr other) {
	std::vector<RobotAgentPtr>::iterator it;
	if (this->isPartOfOrganism()) {
		for (it = this->_organism->agents.begin(); it != this->_organism->agents.end(); it++) {
			if ((*it)->_wm->_agentId == other->_wm->_agentId) {
				return true;
			}
		}
	}
	return false;
}
