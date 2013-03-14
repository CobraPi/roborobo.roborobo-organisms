/*
 * $Id: Fixed32.h 780 2009-02-13 14:01:03Z nicolas $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

// Fixed32.h
// simple class for fixed-point 32 bits value.
// nicolas.bredeche(at)lri.fr, started from Christopher's draft in symbrion repository (rev627)
// started : 20090128
// revision: 20090204.1755
//
//	- 20090128
// 			started from Christopher's draft (?) (taken from Florian repository, rev627 - 19/1/2009)
//			implemented/tested missing operators
//			removed tolerant double/int operations feature (secure)
//			added sign/magnitude/fractional get methods
//			added overflow flag for division by zero
//			corrected arithmetic operations.
//			bit-shift operators.
//	- 20090204
//			added coherent mode for mult/div, approximate flag and PROTECTED_FIXED32 pre-processor option
//
// General notes
//  - This fixed-point 32 bits integer class stick to the basic, but also provide multiplication/division operators with a possible "coherence" checking. All internal computation is integer only. We assume integer size is 32 bits.
//  - internal structure is (in bits): Sign(1) + Magnitude(32-1-FRACTIONALSIZE) + Fractional value (FRACTIONALSIZE)
//  - Precision is defined as "FRACTIONALSIZE" (see below, pre-processor command) -- **don't** go beyond 15 (for 32 bits integer) (technical issue: mult/div operators require that half+ bits left over the fixed point).
//  - fixed-point binary encoding means precision loss.
//  - explicit cast is *forced* to avoid trouble -- e.g. " Fixed32 a = 1 ; a = a + (Fixed32)1 ; a = (Fixed32)0 - a; (...)"
//  - remember that precision is far from perfect with this kind of fixed-point type. ie. "0.01 * 0.01 = 0.0001" may quickly end up being "0.00997925 * 0.00997925 = 0.00009155" with FACTORIALSIZE=15, or even "0.00994873 * 0.00994873 = 0.00006104" with FACTORIALSIZE=14...
//  - WARNING: fixed-point is prone to precision loss and overflow -- IT IS YOUR RESPONSABILITY TO CHECK FOR OVERFLOW UNLESS YOU SET PROTECTED_FIXED32 PRE-PROCESSOR OPTION! (see below for an explanation)
//  - multiplication and division are particularly prone to overflow: see PROTECTED_FIXED32 below
//		- multiplying a and b might results in a number with as many digits as in a+b.
//		- internal storage is such that "1.0" is stored such that it occupies 1 digit + the number of digits for the fractional part (see "FRACTIONALSIZE" value)
//		- all 16+ bits coded number is prone to produce an overflow during multiplication
//		- division is subject to the exact same problem as numerator (dividend) is bit-shifted to the left by FRACTIONALSIZE bits during the process (to avoid loss of fractional part).
//		- NOTE: special operator << (SHR) and >> (SHL) are enabled for multiplication and division by 2. There *much* less prone to overflow (ie. "<<1" overflow only if max magnitude bit is 1, ">>1" loses one bit of information)
// - Word of advise:
//		- one should use this type with caution, set FRACTIONALSIZE to 15 (or 14, or... less), and stick at all costs to value btw [-1,+1].
//		- as a result: the 16-1 upper bits of the 32 bits integer type are used as buffer for securing division and multiplication operations.
//		- do remember to *always* cast other typed values to Fixed32. e.g.: " Fixed32 a = 1 ; a * (Fixed32)10 ; "
//		- divide by zero trigger flag of the same name -- check for it.
//		- (again) be sure to avoid overflow... e.g. if FRACTIONALSIZE = 15, you cannot do "Fixed32 a = 1 ; a * (Fixed32)10 ; " as operator* overflows.
//		- whenever possible, use multiplication/division by 2 though the "<<" and ">>" shift bit operators.
//  - about displaying value. There are two possibility:
//		(1)
//			cout << a.getSign() << a.getMagnitude() << "." ;
//			cout.width(a.getFractionalDigitSize());
//			cout.fill('0');
//			cout << a.getFractional() << endl;
//		(2)
//			cout << a << endl;
//  - multiplication and division operators versus loss of coherence -- PROTECTED_FIXED32 pre-processor option
//		- multiplication and division are both implemented using a right shift that *may* end up in loosing relevant information (ie. heavy right bits)
//		- division is protected (ie. division by zero trigger overflow and return a result where divisor is 1)
//		- as a result, "coherence loss" is bound to happen if the number of coding bits is more than the total number of bits minus 1 (the sign)
//		- in multiplication: both operands are right shifted of FRACTIONAL_SIZE bits. In division: only the dividend is right shifted of FRACTIONAL SIZE.
//		- You should note that if your code *always* stick to value in ]-1,+1[, you will *never* get overflow whatever the operator as long as FRACTIONAL_SIZE is <= 15 bits (as there is till (int_size-sign_bit-fractional_bits=)32-1-15=16 available bits for shifting)
//		- In case you dont know (or want to test) if the range of you number is in ]-1,+1[, you can set the pre-processor symbol to PROTECTED_FIXED32 (see #define below)
//			- when this option is set, mult/div operators try to maintain coherence by loosing precision (ie. left shifting) operands *before* mult/div.
//			- for some calculus, this results in loss of precision in the less important weaker bits rather than in the crucial stronger bits.
//				- (an approximation example is that instead of having 20000.21341 as a result for mult/div, you'll get 20000.)
//				- Then again, the more difficult coherence it is to maintain (ie. size of left shifting), the more precision loss you get - which can still be dramatic for values close to the limit.
//				- The technical reason is that to cope with numbers coded on nearly all bits of the integer storage value space, these values are normalized (through bit-shifting) btw ]-1,+1[, then result is computed, and shifted-back to the correct magnitude of order.
//			- note that on the cortex board, it is still an open issue if extensive use of multiplications/divisions makes double emulation a better choice or not *IF* PROTECTED_FIXED32 is set (ie. approximation takes some time).
//			- there is a speed issue with using this flag. Therefore, if you know for sure that mult/div *never* overflows, dont use this option.
//			- Whenever PROTECTED_FIXED32 is setup, an "approximation" flag if an operation that may lead to an approximation occurs (ie. it does not imply *for sure* that there will be an approximation)
//			- It is the user responsability to check for the approximation flag (through the isApproximate/resetApproximateFlag methods)
//			- general comment: once again, if you want more or less precise computation, stick to ]-1,+1[ numbers. However, as long as overflow is not triggered.
//			- overflow flag is triggered only if overflow occurs. This is a critical issue (ie. numbers so big result cant be approximated in the given number of bits).
//
// TODO list:
//  - implement overflow flag (not implemented yet)
//  - profile code on cortex for speed comparaison btw fixed32 with PROTECTED_FIXED32 option and double emulation. see comment above about mult/div and precision loss.
//	- add const value about MAX and MIN values
//	- implement display method for the cortex
//  - add conversion for integer/float (not sure about this one -- automatic cast may lead to many implementation bugs due to undetected overflows)
//  - switch to a more efficient library (...!)
//		- a possible option is the allegro fixed point math routines (http://www.allegro.cc/manual/api/fixed-point-math-routines/)
//		- however, i'm not sure how allegro perform general multiplication and division operators (*seems* both intel assembly code and sloppy double conversion may be found depending on the target platform -- TO BE CHECKED)
//
//


#ifndef FIXED32_H
#define FIXED32_H

#include "lioutils/lio_global.h"
#include "lioutils/Misc.h"

#ifdef EMBEDDED_VERBOSE
#include "core/Syslog.h"
#endif

#define FRACTIONALSIZE 15 // default: 15 // note: for 32 bits signed integer, a maximum of 15 bits for fractional part is advised (ie. considering value btw -1 and +1, 15*2 bits are needed for multiply and divide if precision loss and overflow is to be avoided)
#define FIXED32_BITSIZE 32 // should *always* be 32 bits (at least on cortex board). (TODO: ensure consistency if not 32 [no priority])
#define PROTECTED_FIXED32 // overflow flag + tries to reduce information loss during multiplication and division -- see comments at the beginning of the file.

class Fixed32
{
	private:
		int32_t value;
		bool _overflow; // divide and multiply may trigger overflow.  // NOT USED FOR THE MOMENT
		bool _divideByZero; // division by zero
		bool _approximate; // if PROTECTED_FIXED32 is set, divide and multiply may trigger _rounded flag. This only means that the result MAY BE rounded (but not for sure). moreover, "rounded" mean approximated.

		static int32_t _mask; // cache value for getFractionalMask()

		int32_t getFractionalMask(void) const; // compute fractional region mask -- done only once at startup.

	public:

		// initialization and casts..

		Fixed32(void);
		Fixed32(const Fixed32& x);
		Fixed32(int x);
		Fixed32(float x);
		Fixed32(double x);
		~Fixed32(void);

		operator float(void) const;
		operator double(void) const;
		operator int(void) const;

		Fixed32& operator=(Fixed32 x);

		// arithmetic operations

		Fixed32 operator+(const Fixed32& x) const;
		Fixed32 operator-(const Fixed32& x) const;
		Fixed32 operator*(const Fixed32& x) const; // may overflow if result from x1 + x2 is too big (ie. added coding bits from both equal to more than the total number of bits available for encoding)
		Fixed32 operator/(const Fixed32& x) const; // may overflow if dividend size too big (ie. more than half the number of bits for encoding)

		void operator++(int x);
		void operator--(int x);

		Fixed32& operator+=(Fixed32 x);
		Fixed32& operator-=(Fixed32 x);
		Fixed32& operator*=(Fixed32 x); // see operator*
		Fixed32& operator/=(Fixed32 x); // see operator/

		Fixed32 operator>>(int x); // preferably use this method for division (by 2^x)
		Fixed32 operator<<(int x); // preferably use this method for multiplication (by 2^x)


		// comparaisons

		bool operator==(const Fixed32& x) const;
		bool operator!=(const Fixed32& x) const;
		bool operator<(const Fixed32& x) const;
		bool operator>(const Fixed32& x) const;
		bool operator<=(const Fixed32& x) const;
		bool operator>=(const Fixed32& x) const;

		// monitoring

		void setRawValue( int __value );
		int getRawValue(void) const; // debug purpose -- return the raw "int" value, ie. same encoding, no fixed point.

		void resetOverflowFlag( ); // set overflow flag to false
		bool isOverflow(void) const; // NOT USED
		void resetDivideByZeroFlag( ); // set dividebyzero flag to false
		bool isDivideByZero(void) const; // flag is triggered if division by zero.
		void resetApproximateFlag( ); // set overflow flag to false
		bool isApproximate(void) const; // NOT USED

		int getFractionalDigitSize() const; // display purpose for the fractional part.

		char getSign() const; // either '-' or '+'
		int getMagnitude() const; // unsigned magnitude value
		int getFractional() const; // unsigned fractional value

		// static Fixed32 xrand( ); // return value in [0,1[

		static void display( Fixed32 __data );

		#ifdef EMBEDDED_VERBOSE
		friend SysLog& operator<< (SysLog& os, Fixed32 __data);
		#else
		friend std::ostream& operator<<(std::ostream& os, Fixed32 __data);
		#endif // EMBEDDED_VERBOSE

};

#endif // FIXED32_H


//-n
