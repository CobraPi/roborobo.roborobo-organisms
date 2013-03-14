/*
 * $Id: Fixed32.cpp 775 2009-02-13 13:08:14Z evert $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

// Fixed32 - signed fixed-point integer
// check header file for comments.

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"


// *
// * type definition
// *
#define	RESOLUTION (1 << (FRACTIONALSIZE))
#define RESOLUTIONF (float)RESOLUTION
#define RESOLUTIOND (double)RESOLUTION

int32_t Fixed32::_mask = 0; // Will be set properly in the first call to getFractionalMask()

// *
// * initialization and casts..
// *

Fixed32::Fixed32(void)
{
	_overflow = false;
	_divideByZero = false;
	value = 0;
}

Fixed32::Fixed32(const Fixed32& x)
{
	_overflow = x._overflow;
	_divideByZero = x._divideByZero;
	value = x.value;
}

Fixed32::Fixed32(int x)
{
	_overflow = false;
	_divideByZero = false;
	value = x << FRACTIONALSIZE;
}

Fixed32::Fixed32(float x)
{
	_overflow = false;
	_divideByZero = false;
	value = int32_t(x * RESOLUTIONF);
}

Fixed32::Fixed32(double x)
{
	_overflow = false;
	_divideByZero = false;
	value = int32_t(x * RESOLUTIOND);
}

Fixed32::~Fixed32(void)
{

}

Fixed32::operator float(void) const
{
//	_overflow = false;
//	_divideByZero = false;
	return float(value) / RESOLUTIONF;
	return 0;
}

Fixed32::operator double(void) const
{
//	_overflow = false;
//	_divideByZero = false;
	return double(value) / RESOLUTIOND;
	return 0;
}

Fixed32::operator int(void) const
{
//	_overflow = false;
//	_divideByZero = false;
	return value >> FRACTIONALSIZE;
}

Fixed32& Fixed32::operator=(Fixed32 x)
{
	_overflow = x._overflow;
	_divideByZero = x._divideByZero;
	value = x.value;
	return *this;
}


// *
// * arithmetic operations
// *


Fixed32 Fixed32::operator+(const Fixed32& x) const
{
	Fixed32 returnValue;
	returnValue.value = value + x.value;
	return returnValue;
}

Fixed32 Fixed32::operator-(const Fixed32& x) const
{
	Fixed32 returnValue;
	returnValue.value = value - x.value;
	return returnValue;
}

/**
 * multiply operator
 * if pre-processor symbol "PROTECTED_FIXED32" exists, overflow is checked and coherence is maintained (possibily at the cost of precision loss)
 * An overflow may occur if the added number of bits for both numbers is more than the possible number of coding bits
 * Coherence is maintained by right shifting operands
 */
Fixed32 Fixed32::operator*(const Fixed32& x) const
{
	Fixed32 returnValue;

#ifdef PROTECTED_FIXED32 	// ensure coherence, but may loose precision, slower method.

	int size1 = getSignificantCodeSize(value,FIXED32_BITSIZE-1);
	int size2 = getSignificantCodeSize(x.value,FIXED32_BITSIZE-1);

	if ( size1 + size2 <= FRACTIONALSIZE * 2 ) // use fractional size as reference -- hypothesis: only operations on ]-1,+1[ numbers may not overflow
		returnValue.value = (value * x.value) >> FRACTIONALSIZE;
	else
	{
		returnValue._approximate = true; // there *may be* an approximation (depending on the fact that lower bits are active or not)

		if ( size1 > FRACTIONALSIZE && size2 > FRACTIONALSIZE ) // tries to keep coherence (note: approximation is not test -- this would slow as you would have to test for disappearing bits)
		{
			returnValue.value =
				(
					( ( value>>(size1-FRACTIONALSIZE) ) * ( x.value>>(size2-FRACTIONALSIZE) ) )
					>> FRACTIONALSIZE
				)
				<< ( size1+size2-2*FRACTIONALSIZE );

			/*
			// DEBUG
			std::cout << "a: " << getStringOfBinaryValue(value,FIXED32_BITSIZE) << " (" << *this << ") -- size: " << size1 << endl;
			std::cout << "b: " << getStringOfBinaryValue(x.value,FIXED32_BITSIZE) << " (" << x << ") -- size: " << size2 << endl;

			int a = value>>(size1-FRACTIONALSIZE);
			int b = x.value>>(size2-FRACTIONALSIZE);
			returnValue.value = ( a * b );

			std::cout << "A: " << getStringOfBinaryValue(a,FIXED32_BITSIZE) << endl;
			std::cout << "B: " << getStringOfBinaryValue(b,FIXED32_BITSIZE) << endl;

			std::cout << "1= " << getStringOfBinaryValue(returnValue.value,FIXED32_BITSIZE) << " = " << returnValue << std::endl;

			returnValue.value =
				returnValue.value
				>> FRACTIONALSIZE;

			std::cout << "2= " << getStringOfBinaryValue(returnValue.value,FIXED32_BITSIZE) << " = " << returnValue << std::endl;

			returnValue.value =
				returnValue.value
				<< ( size1+size2-2*FRACTIONALSIZE );

			std::cout << "3= " << getStringOfBinaryValue(returnValue.value,FIXED32_BITSIZE) << " = " << returnValue << std::endl;
			*/
		}
		else
		{
			if ( size1 > FRACTIONALSIZE )
			{
				returnValue.value =
					(
						( ( value>>(size1-FRACTIONALSIZE) ) * ( x.value ) )
						>> FRACTIONALSIZE
					)
					<< ( size1-FRACTIONALSIZE );
			}
			else
			{
				returnValue.value =
					(
						( ( value) * ( x.value>>(size2-FRACTIONALSIZE) ) )
						>> FRACTIONALSIZE
					)
					<< ( size2-FRACTIONALSIZE );
			}
		}
	}

#else						// fast method, but may loose coherence

	returnValue.value = (value * x.value) >> FRACTIONALSIZE;

#endif

	return returnValue;
}

// PROTECTED division (return 0 and set overflow flag to true)
// possible overflow if dividend uses more that (Max_bits-1)/2 bits
// TODO set overflow flag when needed
Fixed32 Fixed32::operator/(const Fixed32& x) const
{
	Fixed32 returnValue;

	if ( x.value == 0 )
	{
		returnValue._divideByZero = true;
		returnValue.value = 0;
	}
	else
	{

#ifdef PROTECTED_FIXED32 	// ensure coherence, but may loose precision, slower method.

		int size1 = getSignificantCodeSize(value,FIXED32_BITSIZE-1);

		if ( size1 <= FRACTIONALSIZE ) // use fractional size as reference -- hypothesis: only operations with operand btw ]-1,+1[ may not overflow
			returnValue.value = (value << FRACTIONALSIZE) / x.value;
		else
		{
			returnValue._approximate = true; // there *may be* an approximation (depending on the fact that lower bits of operand are active or not)
			// tries to keep coherence (note: approximation is not test -- this would slow as you would have to test for disappearing bits)
			returnValue.value =
				( ( ( value>>(size1-FRACTIONALSIZE) ) << (FRACTIONALSIZE)) / ( x.value ) )
				<< ( size1-FRACTIONALSIZE );
			/*
			std::cout << "\na: " << getStringOfBinaryValue(value,FIXED32_BITSIZE) << " (" << *this << ") -- size: " << size1 << endl;
			int size2 = getSignificantCodeSize(x.value,FIXED32_BITSIZE-1);
			std::cout << "b: " << getStringOfBinaryValue(x.value,FIXED32_BITSIZE) << " (" << x << ") -- size: " << size2 << endl;

			int a = value>>(size1-FRACTIONALSIZE);
			int b = x.value;//>>(size2-FRACTIONALSIZE);
			returnValue.value = ( ( a << FRACTIONALSIZE ) / b );

			std::cout << "A: " << getStringOfBinaryValue(a,FIXED32_BITSIZE) << endl;
			std::cout << "B: " << getStringOfBinaryValue(b,FIXED32_BITSIZE) << endl;

			std::cout << "1= " << getStringOfBinaryValue(returnValue.value,FIXED32_BITSIZE) << " = " << returnValue << std::endl;

			returnValue.value =
				returnValue.value
				<< ( size1-FRACTIONALSIZE );

			std::cout << "2= " << getStringOfBinaryValue(returnValue.value,FIXED32_BITSIZE) << " = " << returnValue << std::endl;
			*/
		}

#else						// fast method, but may loose coherence

		returnValue.value = (value << FRACTIONALSIZE) / x.value;

#endif

	}

	return returnValue;
}

void Fixed32::operator++( int x )
{
	value = value + (1<<FRACTIONALSIZE);
}

void Fixed32::operator--( int x )
{
	value = value - (1<<FRACTIONALSIZE);
}

Fixed32& Fixed32::operator+=(Fixed32 x)
{
	value += x.value;
	return *this;
}

Fixed32& Fixed32::operator-=(Fixed32 x)
{
	value -= x.value;
	return *this;
}

Fixed32& Fixed32::operator*=(Fixed32 x)
{
	//value = (value * x.value) >> FRACTIONALSIZE;
	*this = *this * x;
	return *this;
}

Fixed32& Fixed32::operator/=(Fixed32 x)
{
	*this = *this / x;
	return *this;
}

Fixed32 Fixed32::operator>>(int x)
{
	Fixed32 returnValue;
	returnValue.value = value >> x;
	return returnValue;
}

Fixed32 Fixed32::operator<<(int x)
{
	Fixed32 returnValue;
	returnValue.value = value << x;
	return returnValue;
}



// *
// * comparaisons
// *

bool Fixed32::operator<(const Fixed32& x) const
{
	return value < x.value;
}

bool Fixed32::operator>(const Fixed32& x) const
{
	return value > x.value;
}

bool Fixed32::operator>=(const Fixed32& x) const
{
	return value >= x.value;
}

bool Fixed32::operator<=(const Fixed32& x) const
{
	return value <= x.value;
}

bool Fixed32::operator!=(const Fixed32& x) const
{
	return value != x.value;
}

bool Fixed32::operator==(const Fixed32& x) const
{
	return value == x.value;
}


// *
// * monitoring
// *

void Fixed32::resetApproximateFlag( )
{
	_approximate = false;
}

bool Fixed32::isApproximate() const
{
	return _approximate;
}

void Fixed32::resetOverflowFlag( )
{
	_overflow = false;
}

bool Fixed32::isOverflow() const
{
	return _overflow;
}

void Fixed32::resetDivideByZeroFlag( )
{
	_divideByZero = false;
}

bool Fixed32::isDivideByZero() const
{
	return _divideByZero;
}

char Fixed32::getSign() const
{
	if ( value >= 0)
		return '+';
	else
		return '-';
}

int Fixed32::getMagnitude() const
{
	if ( value >= 0 )
		return value >> FRACTIONALSIZE;
	else
	{
		Fixed32 returnValue = 0;
		returnValue.value = ( ( value - 1 ) >> FRACTIONALSIZE ) + 1;
		return -(returnValue.value);	// return *only* magnitude (ie. take out "sign") -- note: as -0 does not exist, sign has to be treated separately
	}
}

int Fixed32::getFractional() const
{
	int fractionalValue = 0;

	int32_t fractionalRawValueBuffer = value;
	if ( fractionalRawValueBuffer < 0 ) fractionalRawValueBuffer = -fractionalRawValueBuffer;

	fractionalRawValueBuffer &= getFractionalMask();

	for ( int i = 0 ; i != getFractionalDigitSize() ; i++ )
	{
		fractionalRawValueBuffer *= 10;
		fractionalValue = 10*fractionalValue + (fractionalRawValueBuffer >> FRACTIONALSIZE);
		fractionalRawValueBuffer &= getFractionalMask();
	}

	return fractionalValue;
}

int Fixed32::getFractionalDigitSize() const
{
	int32_t mask = getFractionalMask();

	int digitSize = 0;
	while (mask != 0)
	{
		digitSize++;
		mask /= 10;
	}

	return digitSize;
}

void Fixed32::setRawValue( int __value ) // debug
{
	value = __value;
}

int Fixed32::getRawValue() const // debug
{
	return value;
}

//Fixed32 Fixed32::xrand( )
//{
//#ifdef EMBEDDED
//	return ( 0 ); // NOT IMPLEMENTED
//#else
//	Fixed32 v;
//	v.value = rand() & getFractionalMask() ;
//	return ( v );
//#endif //EMBEDDED
//}

int32_t Fixed32::getFractionalMask(void) const
{
	if (_mask == 0)
	{
		for ( int i = 0 ; i != FRACTIONALSIZE ; ++i )
		{
//			int powValue = 1;
//			for ( int j = 0 ; j != i ; j++  )
//				powValue *= 2;
//			_mask += powValue;
			_mask += 1 << i;
		}
	}

	return _mask;
}

void Fixed32::display( Fixed32 __data )
{
#ifndef EMBEDDED_VERBOSE // NOT
	std::cout << __data.getSign() << __data.getMagnitude() << "." ;
	std::cout.width(__data.getFractionalDigitSize());
	std::cout.fill('0');
	std::cout << __data.getFractional();
#endif // EMBEDDED_VERBOSE
}

#ifdef EMBEDDED_VERBOSE
SysLog& operator<<(SysLog& os, Fixed32 __data)
{
	os << __data.getSign() << __data.getMagnitude() << ".";
	os << __data.getFractional();
	return os;
}
#else
std::ostream& operator<<(std::ostream& os, Fixed32 __data)
{
	os << __data.getSign() << __data.getMagnitude() << ".";
	os.width(__data.getFractionalDigitSize());
	os.fill('0');
	os << __data.getFractional();
	return os;
}
#endif // EMBEDDED_VERBOSE



#ifdef UNITTEST
//
// Unit tests for Fixed32 below
//

#include <test/TestHarness.h>

TEST(Counting, Fixed32)
{
	Fixed32 i;
	int k = 0;
	for ( i = (Fixed32)0 ; i != (Fixed32)11 ; i++ )
	{
		CHECK_EQUAL(k, (int) i);
		CHECK_EQUAL(i, (Fixed32)i);
		++k;
	}

	// TODO: pre-increment
}

using std::cout;
using std::endl;

TEST(Ops, Fixed32)
{
	float foo = 0.042;
	Fixed32 a =(Fixed32)(foo);
	CHECK_EQUAL(a, (Fixed32)0.042);

	a = (Fixed32)((int)1);
	CHECK ( a == (Fixed32)((int)1) );

	a = 1;
	int j = (int)a;
	CHECK ( j == 1 );

	a = 1;
	float f = (float)a;
	CHECK ( f == 1.0 );


	// Note that the following fixed-point number is can be expressed as power of 2, which enable an exact encoding.
	// If it cannot be expressed as power of 2 (e.g. 10.12) - error may occured that may be due to precision loss only.
	a = (float)10.5;
	f = a;
	CHECK ( f == 10.5 );

	Fixed32 b = 10;
	CHECK ( b == (Fixed32)10 );

	// cout << "a = 0 , b = 10 , a + b = 10 ";
	a = 0;
	b = 10;
	a = a + b;
	CHECK ( a == (Fixed32)10 );

	// cout << "b = 10 , b * b = 100 ";
	b = 10;
	b = b * b;
	CHECK ( b == (Fixed32)100 );

	//cout << "b = 10 , b++ ";
	b=10;
	b++;
	CHECK ( b == (Fixed32)11 );

	// cout << "b = 20 , b-- ";
	b=20;
	b--;
	CHECK ( b == (Fixed32)19 );

	//cout << "b = 30 , b+=10 , b-=15 ";
	b=30; b+=10; b-=25;
	CHECK ( b == (Fixed32)15 );

	// cout << "b = 14 , b*=2 ";
	b=14; b*=2;
	CHECK ( b == (Fixed32)28 );

	// cout << "b = 14 , a=2 , b = b / a";
	b=14; a=2;
	b = b / a;
	CHECK ( b == (Fixed32)7 );

	//cout << "b = 14 , a=2 , b /= a";
	b=14; a=2;
	b/=a;
	CHECK ( b == (Fixed32)7 )

	//cout << "b = 0.5 , a=0.05 , b = b / a";
	b=0.5; a=0.05;
	b = b / a;
	CHECK ( (int)b == (int)10 );

//	a = 10.12f;
//	cout << "a = 10.12 , print(a) = \"";
//	Fixed32::display(a);
//	cout << "\" -- this example shows precision loss." << endl;

//	a = 1;
//	cout << endl << "Testing std::operator<< : " << a << endl;

	//a = 10;b = 1;b = b/a;
//	a = -2;
//	b = 0.2f;
//	cout << "counting from ";
//	Fixed32::display(a);
//	cout << " to +2.0, stepsize = ";
//	Fixed32::display(b);
//	cout << " : " << endl;
//	for ( int i = 0 ; i != 21 ; i++ )
//	{
//		Fixed32::display(a);
//		cout << "  (" << a.getRawValue() << ")" <<endl;
//		a = a + b;
//	}
//	cout << endl;

//	b.setRawValue(0x00000001);
//	cout << "b = + smallest-possible-value = " ;
//	Fixed32::display(b);
//	cout << " -- (" << b.getRawValue() << ") " << endl;
//	b = (Fixed32)0-b;
//	cout << "b = - smallest-possible-value = " ;
//	Fixed32::display(b);
//	cout << " -- (" << b.getRawValue() << ") " << endl;		cout << endl;

//	a = 3.1415926535;
//	cout << "+PI = ";
//	Fixed32::display(a);
//	cout << endl;
//	a = -3.1415926535;
//	cout << "-PI = ";
//	Fixed32::display(a);
//	cout << endl;
//
//	cout << endl;
	a = -1;
	b = 0;
	//cout << "-1 / 0 : ";
	Fixed32 c = a / b ;
	CHECK ( c.isDivideByZero() );
	c.resetDivideByZeroFlag();

//	cout << endl;
//	a.setRawValue(0x00000001);
//	cout << "smallest-possible-value : ";
//	Fixed32::display(a);

	a = -1;
	b.setRawValue(0x00000001);
	//cout << "-1 / smallest-possible-value : ";
	c = a / b ;
	CHECK ( ! c.isDivideByZero() );

#ifndef EMBEDDED_VERBOSE // NOT
	cout << "divide 0.4 by 0.13 : " << endl;
	a = 0.4;
	b = 0.13;
	c = a / b ;
	Fixed32::display(a);
	cout << " / ";
	Fixed32::display(b);
	cout << " = " ;
	Fixed32::display(c);
	cout << endl;

	cout << endl;
//	cout << "displaying random values : " << endl;
//	for ( int i = 0 ; i != 5 ; i++ )
//	{
//		a = Fixed32::rand();
//		//a = a * (Fixed32)10; // wont work if FRACTIONAL size is >=15
//		Fixed32::display(a);
//		cout << "  ";
//	}
//	cout << endl;

#endif
	//cout << "a = 1 ; a << 2 ; a = ?" << endl;
	a = 1;
	a = a << 2;
	CHECK ( a == (Fixed32)4 );

	//cout << "a = 1 ; a >> 2 ; a = ?" << endl;
	a = 1;
	a = a >> 2;
	CHECK ( a == (Fixed32)0.25);

	//cout << "a = -1 ; a >> 2 ; a = ?" << endl;
	a = -1;
	a = a >> 2;
	CHECK ( a == (Fixed32)-0.25);

#ifndef EMBEDDED_VERBOSE // NOT
	a = 0;
	b = 0.01;
	c = 0.01;
	a = b * c;
	cout << "a = " << ((double)b) << " * " << ((double)c) << " = " << ((double)a) << endl;

	cout << endl;
	a = 0;
	b = 2;
	c = 2;
	a = b * c;
	cout << "a = " << ((double)b) << " * " << ((double)c) << " = " << ((double)a) << endl;


	cout << endl;
	a = 0;
	b = 20000;
	c = 2;
	a = b * c;
	cout << "a = " << ((double)b) << " * " << ((double)c) << " = " << ((double)a) << endl;

	cout << endl;
	a = 4096;//2800
	b = a;
	c = 1;
	a *= c;
	cout << "a = " << b ;
	cout << " ; a*=" << c << " = " << ((double)a) << endl;

	cout << endl;
	a = 2009;
	b = a;
	c = 14.5;
	a *= c;
	cout << "a = " << b ;
	cout << " ; a*=" << c << " = " << ((double)a) << endl;

	cout << endl;
	a = 1492;
	b = a;
	c = 0.02;
	a *= c;
	cout << "a = " << b ;
	cout << " ; a*=" << c << " = " << ((double)a) << endl;

	cout << endl;
	a = -0.05;
	b = a;
	c = -1492;
	a *= c;
	cout << "a = " << b ;
	cout << " ; a*=" << c << " = " << ((double)a) << endl;
#endif
}

#endif
//-n


