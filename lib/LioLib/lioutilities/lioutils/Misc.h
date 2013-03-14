/*
 * $Id: $
 *
 *  Created on: 4 f�vr. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#ifndef MISC_H
#define MISC_H

#include "lioutils/lio_global.h"

int getSignificantCodeSize(int __value, int __size); // call with size-1 if you want to ignore bit sign.

#ifndef EMBEDDED_VERBOSE // NOT
std::string getStringOfBinaryValue( int __value, int __size );
#endif

#endif // MISC_H
