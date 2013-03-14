/*
 * $Id: $
 *
 *  Created on: 28 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

// this header file should contain mostly pre-processing commands for high-level configuration
// you should include it first hand in all source files in LIO. Read the following for details (including the external header policy).

#ifndef LIO_GLOBAL_H__
#define LIO_GLOBAL_H__


// =-=-=-=-=-= Precompiler commands

// SUMMARY OF PREPROCESSOR COMMANDS:
//  DEBUG: various debug only features. E.g.: Fixed32 always checks for overflow. (NOT IMPLEMENTED)
//  EMBEDDED:  assume embedded envt (ie. fixed-point integer)
//  EMBEDDED VERBOSE: assume *really* embedded, ie. do not use non embedded commands (e.g. iostream include, std::cout, etc.)

#ifdef UNITTEST
#warning Building tests
#endif

// * Define the EMBEDDED symbol to compile on the cortex.
// You may not define it if you're compiling/running on a laptop/desktop computer.
// Example of use: if EMBEDDED is defined, large system library may not be included to spare memory space. (useful for cross-development/testing btw cortex and laptop)
// EMBEDDED_VERBOSE is used for the verbose mode so you can choose btw cortex envt or laptop/desktop envt monitoring for verbose (e.g.: OLED vs console display).
// Both EMBEDDED and EMBEDDED_VERBOSE must be defined for running on the cortex board

//#define EMBEDDED  // switch to embedded running mode, e.g. fixed-point 32 bits integer data (alternative is: laptop/desktop envt)
//#define EMBEDDED_VERBOSE // switch to embedded verbose mode, ie. display for cortex (alternative is: verbose mode for laptop/desktop envt)

// =-=-=-=-=-= setting data type (fixed point 32 bits integer OR double)

// * switching btw data type (cortex should be fixed32).
#ifdef EMBEDDED
class Fixed32;
typedef Fixed32 LIOReal;
#else // EMBEDDED
typedef double LIOReal;
#endif // EMBEDDED

#ifdef EMBEDDED_VERBOSE
#include <core/SysLog.h>
 inline SysLog& getLog(void) { return LOG; }

#else
#include <iostream>
  inline std::ostream& getLog(void) { return std::cout; }
#include <cstdlib>
#endif // EMBEDDED_VERBOSE

// =-=-=-=-=-= INCLUDES
// NOTE: you should include all needed external header files here (e.g. stdint.h), then include only global.h in your files

//#define

#ifndef USE_STDINT
 #ifndef _INT32_T
 #define _INT32_T

  //typedef	int			int32_t; // assume 32 bits...
  // IF TARGET CORTEX:
  #include "inttypes.h"
 #endif // _INT32_T
#else // USE_STDINT
 #include <stdint.h>
 #include <cmath>
#endif // USE_STDINT

// =-=-=-=-=-= MATH

#define EULER18  2.718281828459045234
#define PI18     3.141592653589793238

// =-=-=-=-=-= Useful macros.

#define SwapInteger(swapIntegerValueOne,swapIntegerValueTwo) int swapIntegerResultingValue;swapIntegerResultingValue=swapIntegerValueOne;swapIntegerValueOne=swapIntegerValueTwo;swapIntegerValueTwo=swapIntegerResultingValue;
#define GetArrayLength(array) ( sizeof(array) / sizeof(array[0]) ) // will work for all fixed size array. E.g.: int[14] a; getArrayLength(a);

#endif // LIO_GLOBAL_H__

//-n
