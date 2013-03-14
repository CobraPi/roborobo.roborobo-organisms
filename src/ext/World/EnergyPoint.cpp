#include "World/EnergyPoint.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "Utilities/SDL_gfxPrimitives.h"

#include "World/World.h"

int EnergyPoint::_nextId = 0;

EnergyPoint::EnergyPoint()
{
	setColor(0xeab71fff); //Setting the color of the EnergyPoint with a default value

    initialize();
}

EnergyPoint::EnergyPoint(int id)
{
	_id = id;

	_fixedLocation = true;
	_active = true;

	setColor(0xeab71fff); //Setting the color of the EnergyPoint with a default value

    initialize();
}

EnergyPoint::EnergyPoint(int id,Uint32 color)
{
	_id = id;

	_fixedLocation = true;
	_active = true;

	setColor(color); //Setting the color of the EnergyPoint with a default value

    initialize();
}


EnergyPoint::~EnergyPoint()
{
	//nothing to do
}

void EnergyPoint::initialize(){
    double x = (rand() % (gAreaWidth-20)) + 10;
	double y = (rand() % (gAreaHeight-20)) + 10;

    std::string s = "";
	s += "energy[";
	std::stringstream out;
	out << _id;
	s += out.str();
	s += "].x";

    bool randomStart = false;
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(x, gProperties.getProperty( s ), std::dec);
	}
	else
	{
		if ( gVerbose )
		{
			std::cout << "[warning] Energy point #" << _id << " - X coordinate not found, pick random value." << std::endl;
		}
		x = (rand() % (gAreaWidth-20)) + 10;
        randomStart = true;
	}

	s = "energy[";
	s += out.str();
	s += "].y";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(y, gProperties.getProperty( s ), std::dec);
	}
	else
	{
		if ( gVerbose )
		{
			std::cout << "[warning] Energy point #" << _id << " - Y coordinate not found, pick random value." << std::endl;
		}
		y = (rand() % (gAreaHeight-20)) + 10;
        randomStart = true;
	}

	setPosition(Point2d(x,y));


    while (randomStart && isCollision()) {
		//std::cout << "Collision detected on " << x << ", " << y << " rerolling" << std::endl;
		x = (int) 20 + (ranf() * (double) (gSpawnWidth));
		y = (int) 20 + (ranf() * (double) (gSpawnHeight));

        setPosition(Point2d(x,y));
	}

	_id = EnergyPoint::_nextId; // set unique id.
	EnergyPoint::_nextId++; // update id reference.

	_fixedLocation = false;
	_active = true;

	gProperties.checkAndGetPropertyValue("initLock", &_initLock, true);
	_key = _initLock;

    gProperties.checkAndGetPropertyValue("gEnergyPointRadius", &gEnergyPointRadius, true);
	_radius = gEnergyPointRadius;

	gProperties.checkAndGetPropertyValue("iterationMax", &_iterationMax, true);

	_respawnLag = gEnergyPointRespawnLagMaxValue ;
	_internLagCounter = 0;
	
	_energyPointValue = gEnergyPointValue;
	_energyPointValueIsLocal = false; // use gEnergyPointValue

	_respawnMethodIsLocal = false; // use gEnergyPointRespawnLagMaxValue
	_energyPointRespawnLagMaxValue = gEnergyPointRespawnLagMaxValue; // default, not used if _respawnMethodIsLocal

	gProperties.checkAndGetPropertyValue("VisibleEnergyPoint", &_visible, true);
	if ( _visible)
	{
		display();
	}
}

bool EnergyPoint::isCollision(){
	if (
		(getPosition().x - _radius < 0) || (getPosition().x + _radius >= gAreaWidth) ||
		(getPosition().y - _radius < 0) || (getPosition().y + _radius >= gAreaHeight)
		) {
		// * collision with border
		return true;
    } else {
        for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius; xColor++)
        {
            for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor++)
            {
                if ((sqrt ( pow (xColor- getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius
                    && getPixel32(gEnergyImage , xColor, yColor) != SDL_MapRGB(gEnergyImage->format, 0xFF, 0xFF, 0xFF ))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void EnergyPoint::display()
{
	for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius; xColor++)
	{
		for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor ++)
		{
			if ((sqrt ( pow (xColor-getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius)
			{
				pixelColor(gBackgroundImage, xColor, yColor, getColor());
                pixelColor(gEnergyImage, xColor, yColor, getColor());
			}
		}
	}
}

void EnergyPoint::hide()
{
	Uint32 color = 0xffffffff;
	for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius ; xColor++)
	{
		for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor ++)
		{
			if ((sqrt ( pow (xColor-getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius)
			{
				pixelColor(gBackgroundImage, xColor, yColor, color);
                pixelColor(gEnergyImage, xColor, yColor, color);
			}
		}
	}
    
    // re-display energypoints that we overlapped
    for(int i=0;i<gEnergyPoints.size();i++){
        if(gEnergyPoints[i]._active && gEnergyPoints[i]._id != this->_id&& getEuclidianDistance(gEnergyPoints[i].getPosition().x, gEnergyPoints[i].getPosition().y, this->getPosition().x, this->getPosition().y) < (gEnergyPoints[i]._radius + this->_radius)){
            gEnergyPoints[i].display();
        }
    }
}

int EnergyPoint::getRespawnLag()
{
	return gEnergyPointRespawnLagMaxValue;
}

void EnergyPoint::setActiveStatus( bool __value )
{
	if ( __value == true )
	{
		_active = true;
		if ( _visible )
		{
			display();
		}
	}
	else
	{	
		_active = false;
	
		if ( _visible )
		{
			hide();
		}
		if(gVerbose)
			gLogFile << gWorld->getIterations() << " :: EnergyPoint nb." << _id << " #internLag: " << _respawnLag << std::endl; // This is not relevant - internLag does not exist anymore, this should be either _respawnLag or gEnergyPointRespawnLagMaxValue (ie. time to respawn)
	}	
}

bool EnergyPoint::getActiveStatus()
{
	return _active;
}

float EnergyPoint::getKey()
{
	return _key;
}

void EnergyPoint::setKey(float key)
{
	_key = key;
}

void EnergyPoint::setRespawnLag( int __lag )
{
	_respawnLag = __lag;
}

int EnergyPoint::getRespawnLagMaxValue( )
{
	return _energyPointRespawnLagMaxValue;
}

void EnergyPoint::setRespawnLagMaxValue( int __lagMaxValue )
{
	setRespawnLagMethodIsLocal(true); // automatically set-up local value (if needed)
	_energyPointRespawnLagMaxValue = __lagMaxValue;
}

void EnergyPoint::setRespawnLagMethodIsLocal( bool __flag )
{
	_respawnMethodIsLocal = __flag;
}

bool EnergyPoint::getRespawnLagMethodIsLocal()
{
	return 	_respawnMethodIsLocal;
}

bool EnergyPoint::getEnergyPointValueIsLocal()
{
	return _energyPointValueIsLocal;
}

void EnergyPoint::setEnergyPointValueIsLocal( bool __flag )
{
	_energyPointValueIsLocal = __flag;
}

int EnergyPoint::getEnergyPointValue()
{
	return _energyPointValue;
}

void EnergyPoint::setEnergyPointValue( int __value )
{
	setEnergyPointValueIsLocal(true);
	_energyPointValue = __value;
}

void EnergyPoint::step()
{
	if ( !_active ) // case: harvested. respawn delay?
	{
		_internLagCounter++;

		if (_respawnLag != -1 &&  _internLagCounter >= _respawnLag ) // Note: in current implementation, _respawnLag is equal to gEnergyPointRespawnLagMaxValue.
		{
			if ( _respawnMethodIsLocal == true )
				_respawnLag = _energyPointRespawnLagMaxValue; // each energy point gets its own respawn lag
			else
				_respawnLag = gEnergyPointRespawnLagMaxValue; // all energy points share the respawn lag value

			_internLagCounter = 0;
			_active = true;

			if ( ! _fixedLocation )
			{
				double x = (rand() % (gAreaWidth-20)) + 10;
				double y = (rand() % (gAreaHeight-20)) + 10;
				setPosition(Point2d(x,y));

                bool randomStart = true;
                while (randomStart && isCollision()) {
                    //std::cout << "Collision detected on " << x << ", " << y << " rerolling" << std::endl;
                    x = (int) 20 + (ranf() * (double) (gSpawnWidth));
                    y = (int) 20 + (ranf() * (double) (gSpawnHeight));

    				setPosition(Point2d(x,y));
                }
			}
			if ( _visible )
			{
				display();
			}
		}
	}
	else
	{
		if ( _visible && gDisplayMode == 0 )
		{
			// NOTE: calling the display method, re-display the food image (but have
			// no impact on simulation whatsoever as food harvesting is based on 
			// computed euclidian distance btw agents and energy points). This is used
			// to guarantee a "nice and clean" display as overlapping energy points 
			// may result in cropped remaining points whenever a point is taken (ie.
			// the actual implementation implies deleting the energy point region
			// that was harvested, without any regards to overlaps).
			// IT IS IMPORTANT TO NOTE THAT SYSTEMATIC CALLS TO DISPLAY MAY 
			// SIGNIFICANTLY SLOW DOWN THE SIMULATION! As a consequence, it should
			// be called with care, and should NOT be called if not be called when
			// the simulation is running fast (e.g. batch mode).
			// Current implementation: 
			//	calls only if display mode is normal speed
			//	and if property "EnergyPoints_alwaysRender" is true
			
			if ( gEnergyPoints_alwaysRender ) // NOTE: the hide() method but be called externally (otw: displayed points may not exist anymore)
				display();
		}
	}
}


void EnergyPoint::setFixedLocationStatus( bool __value )
{
	_fixedLocation = __value;
}

bool EnergyPoint::getFixedLocationStatus()
{
	return _fixedLocation;
}

double EnergyPoint::getRadius(){
	return _radius;
}

bool EnergyPoint::getVisibility(){
	return _visible;
}

void EnergyPoint::setVisibility(bool visible){
	_visible=visible;
}

int EnergyPoint::getID(){
	return _id;
}

