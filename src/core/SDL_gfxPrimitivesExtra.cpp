/*
 *  SDL_gfxPrimitivesExtra.cpp
 *  roborobo
 *
 *  Created by Nicolas on 05/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/SDL_gfxPrimitivesExtra.h"



/**
	trace ray from (x1,y1) to (x2,y2) whatever the obstacles.
*/
void traceRayRGBA(SDL_Surface * image, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	//lineRGBA(image, x1, y1, x2, y2, 255 , 255 , 255 , 255); // nearly equivalent (small algorithmic differences)

	Uint32 pixelColor =SDL_MapRGB( image->format, r, g, b );
	
	if ( abs(x1-x2) > abs (y1-y2) )
	{
		int it;
		
		double dy = double(y1-y2) / double(x1-x2);

		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = x1 ; x != x2 ; x = x + it )
		{
			putPixel32secure( image, x, (int)(yReal+0.5) , pixelColor );
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = double(x1-x2) / double(y1-y2);

		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = y1 ; y != y2 ; y = y + it )
		{
			putPixel32secure( image, (int)(xReal+0.5), y , pixelColor );
			xReal += dx;
		}
	}

}

/**
 * Copied from the RoboRobo branch of Nikita
 *
 * @param surface
 * @param sensorRay
 * @param ranges
 * @param pixels
 * @return
 */
int castSensorRay(SDL_Surface* surface, SensorRay sensorRay, int* ranges, int* pixels) {
    double x1 = sensorRay.x1;
    double y1 = sensorRay.y1;
    double x2 = sensorRay.x2;
    double y2 = sensorRay.y2;
    double maxRange = sensorRay.maxRange;

	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);

	//Convert the pixels to 32 bit
	Uint32 *pixelData = (Uint32 *)surface->pixels;
    Uint32 activePixel;
    Uint32 regionPixel = G_COLOR_WHITE;
	Uint8 r, g, b;
	int count = 0;
	bool isObstacleCollision = false;

	int xInt, yInt, dxInt, dyInt; double xReal = x1, yReal = y1, dxReal, dyReal, xDist, yDist;

    dxInt = 1;
    dyInt = 1;
	dxReal = (x2 - x1) / (y2 - y1);
	dyReal = 1 / dxReal;

	if (abs(dxReal) > 1) {
	    if (x1 > x2) {
	        dxInt = -1;
	        dyReal = -dyReal;
        }
		for (xInt = (int)(x1 + 0.5); xInt != (int)(x2 + 0.5); xInt += dxInt, yReal += dyReal) {
		    yInt = (int)(yReal + 0.5);
			activePixel = pixelData[yInt * (surface->w) + xInt];
			// If current pixel is different from ones visited previously ...
			if (activePixel != regionPixel) {
                if (activePixel == G_COLOR_WHITE) {
                    regionPixel = G_COLOR_WHITE;
                } else {
                    // Then we have a collision, so we are going to record collision range ...
                    xReal = (double)xInt;
                    xDist = xReal - x1;
                    yDist = yReal - y1;
                    ranges[count] = sqrt(xDist * xDist + yDist * yDist);
                    // And pixel data at the collision point ...
                    SDL_GetRGB(activePixel, gEnvironmentImage->format, &r, &g, &b);
                    pixels[count++] = (r << 16) + (g << 8) + b;
                    // Stop scanning if collided with obstacle, but continue scanning if it was a puck.
                    if (r != 0xFF) {isObstacleCollision = true; break;}
                    regionPixel = activePixel;
                }
			}
		}
	} else {
	    if (y1 > y2) {
	        dyInt = -1;
	        dxReal = -dxReal;
	    }
		for (yInt = (int)(y1 + 0.5); yInt != (int)(y2 + 0.5); yInt += dyInt, xReal += dxReal) {
		    xInt = (int)(xReal + 0.5);
			activePixel = pixelData[yInt * (surface->w) + xInt];
			// If current pixel is different from ones visited previously ...
			if (activePixel != regionPixel) {
                if (activePixel == G_COLOR_WHITE) {
                    regionPixel = G_COLOR_WHITE;
                } else {
                    // Then we have a collision, so we are going to record collision range ...
                    yReal = (double)yInt;
                    xDist = xReal - x1;
                    yDist = yReal - y1;
                    ranges[count] = sqrt(xDist * xDist + yDist * yDist);
                    // And pixel data at the collision point ...
                    SDL_GetRGB(activePixel, gEnvironmentImage->format, &r, &g, &b);
                    pixels[count++] = (r << 16) + (g << 8) + b;
                    // Stop scanning if collided with obstacle, but continue scanning if it was a puck.
                    if (r != 0xFF) {isObstacleCollision = true; break;}
                    regionPixel = activePixel;
                }
			}
		}
	}

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);

    // If we recorded no collisions while scanning (even if there were some collisions with pucks).
    if (!isObstacleCollision) {
	    if (maxRange != -1) {
		    ranges[count] = maxRange;
	    } else {
            xDist = xReal - x1;
            yDist = yReal - y1;
            ranges[count] = sqrt(xDist * xDist + yDist * yDist);
        }
        pixels[count++] = 0xFFFFFF;
    }

    return count;
}

/**
	cast ray from (x1,y1) to (x2,y2). Stops whenever ray encounters something. (x2,y2) are update with point of contact
	__maxValue is the maximum distance possible -- ie. if no collision during ray casting (makes it possible to return an exact value without the cost of distance (with sqrt) computation)
*/
int castSensorRay(SDL_Surface * image, double x1, double y1, double *x2pt, double *y2pt, int __maxValue )
{	
	double x2 = *x2pt;
	double y2 = *y2pt;
	
	bool isCollision = false; // check collision btw sensor ray and object.
	
	if ( abs(x1-x2) > abs (y1-y2) )
	{
		int it;
		
		double dy = (y1-y2) / (x1-x2);

		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = (int)(x1+0.5) ; x != (int)(x2+0.5) ; x = x + it )
		{
			if ( getPixel32 ( image, x, (int)(yReal+0.5) ) != SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) )
			{
				*x2pt = (double)x;
				*y2pt = yReal;
				isCollision = true;
				break;
			}
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = (x1-x2) / (y1-y2);

		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = (int)(y1+0.5) ; y != (int)(y2+0.5) ; y = y + it )
		{
			if ( getPixel32 ( image, (int)(xReal+0.5), y ) != SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) )
			{
				*x2pt = xReal;
				*y2pt = (double)y;
				isCollision = true;
				break;
			}
			xReal += dx;
		}
	}

	if ( isCollision == false && __maxValue != -1 )
		return __maxValue;
	else
		return sqrt ( ( x1 - *x2pt ) * ( x1 - *x2pt ) + ( y1 - *y2pt ) * ( y1 - *y2pt ) );
    // should be equal to gSensorRange; // no hit
}



