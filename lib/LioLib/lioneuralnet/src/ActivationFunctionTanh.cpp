/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/lio_global.h"
#include "lioutils/Mathlib.h"

#include "activationfunctions/ActivationFunctionTanh.h"


LIOReal ActivationFunctionTanh::apply( LIOReal __value )
{
	return ( Mathlib::getTanh(__value) );
}

//-n
