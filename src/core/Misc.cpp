/*
 *  Misc.cpp
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"

// useful functions. 
// see header file for description and credits/sources.



double getGaussianRand(double m, double s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {		
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( ( w >= 1.0 ) || ( w == 0.0 ) );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}


double getEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(getSquaredEuclidianDistance(x1, y1, x2, y2));
}

double getSquaredEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return ( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
}

double getEuclidianDistance(Point2d p1, Point2d p2)
{
	return sqrt( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

double getMaximumDistance(){
	return sqrt (pow(gEnvironmentImage->w,2)+ pow(gEnvironmentImage->h,2));
}


double computeModulo( double a , double b )
{
	int result = static_cast<int>( a / b );
	return a - static_cast<double>( result ) * b;
}


std::string getCurrentTimeAsReadableString()
{
	// --- get current time information
	
	time_t now = time(0);
	char timestamp[19] = "";
	//strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	std::string s = timestamp;
	
	// --- get milliseconds resolution (note: could be used to replace code block above - left for tutorial)
	
	struct timeval now2;
    long mtime;
    gettimeofday(&now2, NULL);
    mtime = ((now2.tv_sec) * 1000 + now2.tv_usec/1000.0) + 0.5;
	mtime = mtime - (mtime/1000)*1000;
	//printf("Elapsed time: %ld milliseconds\n", mtime);

	// -- wrap up display

	if ( mtime < 100 )
	{
		s+="0";
		if ( mtime < 10 )
			s+="0";
	}	
	s += convertToString(mtime) + "ms";
		
	return s;

}

std::string convertToString( int __value )			
{
	std::string s;
	std::stringstream sOutTmp;
	sOutTmp << __value;
	s = sOutTmp.str();
	
	return s;
}
			
