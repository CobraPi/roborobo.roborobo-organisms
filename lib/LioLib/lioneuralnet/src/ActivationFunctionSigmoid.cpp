/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/lio_global.h"

#include "lioutils/Mathlib.h"

#include "activationfunctions/ActivationFunctionSigmoid.h"



LIOReal ActivationFunctionSigmoid::apply( LIOReal __value )
{
	return ( ((LIOReal)1.0+Mathlib::getTanh(__value))/(LIOReal)2.0 );
}

//-n
