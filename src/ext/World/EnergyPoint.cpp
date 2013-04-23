#include "World/EnergyPoint.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "Utilities/SDL_gfxPrimitives.h"

#include "World/World.h"
#include "World/ResourceFactory.h"

int EnergyPoint::_nextId = 0;

EnergyPoint::EnergyPoint(SDL_Surface *surface)
{
	setColor(0xeab71fff); //Setting the color of the EnergyPoint with a default value

    initialize(surface);
}

EnergyPoint::EnergyPoint(int id, SDL_Surface *surface)
{
	_id = id;

	_fixedLocation = true;
	active = true;

	setColor(0xeab71fff); //Setting the color of the EnergyPoint with a default value

    initialize(surface);
}

EnergyPoint::EnergyPoint(int id, SDL_Surface *surface,Uint32 color)
{
	_id = id;

	_fixedLocation = true;
	active = true;

	setColor(color); //Setting the color of the EnergyPoint with a default value

    initialize(surface);
}


EnergyPoint::~EnergyPoint()
{
	//nothing to do
}

void EnergyPoint::initialize(SDL_Surface *surface){
    double x = (rand() % (gAreaWidth-20)) + 10;
	double y = (rand() % (gAreaHeight-20)) + 10;

    _id = EnergyPoint::_nextId; // set unique id.
	EnergyPoint::_nextId++; // update id reference.
    
	_fixedLocation = false;
	active = true;
    
    gProperties.checkAndGetPropertyValue("gEnergyPointRadius", &gEnergyPointRadius, true);
	_radius = gEnergyPointRadius;
    
	gProperties.checkAndGetPropertyValue("initLock", &_initLock, true);
	_key = _initLock;
    
	gProperties.checkAndGetPropertyValue("iterationMax", &_iterationMax, true);
    
	_respawnLag = gEnergyPointRespawnLagMaxValue ;
	_internLagCounter = 0;
	
	_energyPointValue = gEnergyPointValue;
	_energyPointValueIsLocal = false; // use gEnergyPointValue
    
	_respawnMethodIsLocal = false; // use gEnergyPointRespawnLagMaxValue
	_energyPointRespawnLagMaxValue = gEnergyPointRespawnLagMaxValue; // default, not used if _respawnMethodIsLocal
    
	gProperties.checkAndGetPropertyValue("VisibleEnergyPoint", &_visible, true);
    
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
    while (randomStart && isCollision(surface)) {
		//std::cout << "Collision detected on " << x << ", " << y << " rerolling" << std::endl;
		x = (int) 20 + (ranf() * (double) (gSpawnWidth));
		y = (int) 20 + (ranf() * (double) (gSpawnHeight));

        setPosition(Point2d(x,y));
	}
    
    display(surface);
}

bool EnergyPoint::isCollision(SDL_Surface *surface){
	if (
		(getPosition().x - _radius < 0) || (getPosition().x + _radius >= gAreaWidth) ||
		(getPosition().y - _radius < 0) || (getPosition().y + _radius >= gAreaHeight)
		) {
		// * collision with border
		return true;
    } else {
        Uint32 white = SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF );
        for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius; xColor++)
        {
            for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor++)
            {
                if ((sqrt ( pow (xColor- getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius
                    && getPixel32(surface , xColor, yColor) != white)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void EnergyPoint::display(SDL_Surface *surface)
{
    if(_visible){
        for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius; xColor++)
        {
            for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor ++)
            {
                if ((sqrt ( pow (xColor-getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius)
                {
                    pixelColor(surface, xColor, yColor, getColor());
                    pixelColor(gBackgroundImage, xColor, yColor, getColor());
                }
            }
        }
    }
}

void EnergyPoint::hide(SDL_Surface *surface)
{
	Uint32 color = 0xffffffff;
    if(_visible){
        for (int xColor = getPosition().x - _radius ; xColor < getPosition().x + _radius ; xColor++)
        {
            for (int yColor = getPosition().y - _radius ; yColor < getPosition().y + _radius; yColor ++)
            {
                if ((sqrt ( pow (xColor-getPosition().x,2) + pow (yColor - getPosition().y,2))) < _radius)
                {
                    pixelColor(surface, xColor, yColor, color);
                    pixelColor(gBackgroundImage, xColor, yColor, color);
                }
            }
        }
    }
}

int EnergyPoint::getRespawnLag()
{
	return gEnergyPointRespawnLagMaxValue;
}

/*void EnergyPoint::setActiveStatus( bool __value )
{
	if ( __value == true )
	{
		active = true;
	}
	else
	{	
		active = false;
        
		if(gVerbose)
			gLogFile << gWorld->getIterations() << " :: EnergyPoint nb." << _id << " #internLag: " << _respawnLag << std::endl; // This is not relevant - internLag does not exist anymore, this should be either _respawnLag or gEnergyPointRespawnLagMaxValue (ie. time to respawn)
	}
}*/



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

void EnergyPoint::step(SDL_Surface *surface)
{
	if ( !active ) // case: harvested. respawn delay?
	{
		_internLagCounter++;

		if (_respawnLag != -1 &&  _internLagCounter >= _respawnLag ) // Note: in current implementation, _respawnLag is equal to gEnergyPointRespawnLagMaxValue.
		{
			if ( _respawnMethodIsLocal == true )
				_respawnLag = _energyPointRespawnLagMaxValue; // each energy point gets its own respawn lag
			else
				_respawnLag = gEnergyPointRespawnLagMaxValue; // all energy points share the respawn lag value

			_internLagCounter = 0;
			active = true;

			if ( ! _fixedLocation )
			{
				double x = (rand() % (gAreaWidth-20)) + 10;
				double y = (rand() % (gAreaHeight-20)) + 10;
				setPosition(Point2d(x,y));

                bool randomStart = true;
                while (randomStart && isCollision(surface)) {
                    //std::cout << "Collision detected on " << x << ", " << y << " rerolling" << std::endl;
                    x = (int) 20 + (ranf() * (double) (gSpawnWidth));
                    y = (int) 20 + (ranf() * (double) (gSpawnHeight));

    				setPosition(Point2d(x,y));
                }
			}
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

