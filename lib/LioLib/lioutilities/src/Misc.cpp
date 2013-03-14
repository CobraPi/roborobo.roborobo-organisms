/*
 * $Id: $
 *
 *  Created on: 4 f�vr. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/Misc.h"

int getSignificantCodeSize(int __value, int __size)
{
	if ( __value < 0 ) __value = -__value;

	//int mask = 0x40000000; // WARNING! assume 32 bits! ignore sign. (TODO: explicit check)

	int codingSize; // ie. number of bits needed to code this integer value.
	for ( codingSize = __size ; codingSize >= 0 ; codingSize-- ) // assume 32 bits. ignore sign.
	{
		if ( ( __value & (1 << codingSize) ) )
		{
			codingSize++;
			break;
		}
	}

	return codingSize;
}


#ifndef EMBEDDED_VERBOSE // NOT
using std::string;
string getStringOfBinaryValue( int __value, int __size )
{
	string returnString = "";

	for( int i = __size ; i >= 0 ; i-- )
		if( ( __value & (1 << i) ) )
			returnString += "1";
		else
			returnString += "0";

		return returnString + "b";
}
#endif


//-n
