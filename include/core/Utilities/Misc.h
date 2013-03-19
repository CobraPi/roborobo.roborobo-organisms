/*
 *  Misc.h
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MISC_H
#define MISC_H

#include <vector> 
#include <cmath> 

#include <string>
#include <sstream>
#include <iostream>

#include <cstdlib> // rand and RAND_MAX

#include <sys/time.h>

#define swapInteger(a,b) {int tmp;tmp=a;a=b;b=tmp;}


#define ranf() \
  ((double)rand()/(1.0+(double)RAND_MAX)) // Uniform from interval [0,1) */


// A useful struct/class for storing 2D real coordinates.
class Point2d
{
	public:
    double x;
    double y;
    
		Point2d()
		{y=0;x=0;};
		Point2d(double inX, double inY){
			x = inX;
			y = inY;
		};
    
    Point2d(const Point2d &other){
        x = other.x;
        y = other.y;
    }
    
    Point2d& operator =(const Point2d& v)
    {
        x = v.x;
        y = v.y;
        return (*this);
    }

    inline double distanceSquared(Point2d p){
        double xdiff = x - p.x;
        double ydiff = y - p.y;
        double xsquare = xdiff * xdiff;
        double ysquare = ydiff * ydiff;
        
        return xsquare + ysquare;
    }
    
    inline double distance(Point2d p){
        return std::sqrt(distanceSquared(p));
    }
    
    inline double distanceX(Point2d p){
        return x - p.x;
    }

    inline double distanceY(Point2d p){
        return y - p.y;
    }
    
    friend std::ostream& operator<< (std::ostream &stream, Point2d point){
        stream << point.x << ", " << point.y;
        return stream;
    }
};



/* Credits:
	boxmuller.c           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
			  this software for any application provided this
			  copyright notice is preserved.
			  http://www.taygeta.com/random/gaussian.html
			  http://www.bearcave.com/misl/misl_tech/wavelets/hurst/random.html
*/
double getGaussianRand (double m, double s); // box-muller



// Cast string to value.
//
// cite: "the third parameter of from_string() should be one of std::hex, std::dec or std::oct"
// usage:
//  convertFromString<int>(i, std::string("ff"), std::hex))
//  convertFromString<float>(f, std::string("123.456"), std::dec))
// return: success/failure as boolean value
// source: http://www.codeguru.com/forum/showthread.php?t=231054
template <class T>
bool convertFromString(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

// return euclidian distance btw two 2D points
double getEuclidianDistance(double x1, double y1, double x2, double y2);

// return the square of the euclidian distance btw two 2D points. 
// hint: comparing squared values is faster than using squareroot...
double getSquaredEuclidianDistance(double x1, double y1, double x2, double y2);

// return euclidian distance btw two 2D points of type Point2d
double getEuclidianDistance(Point2d p1, Point2d p2);

// return the maximum euclidian distance possible in current arena sqrt(w^2+h^2)
double getMaximumDistance();

double computeModulo(double a, double b);

// return current time in a string, with readable format - e.g. 20100526-10h12m08s
// check: http://www.cplusplus.com/reference/clibrary/ctime/strftime/
std::string getCurrentTimeAsReadableString();

// check: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
std::string convertToString( int __value );

#endif // MISC_H

