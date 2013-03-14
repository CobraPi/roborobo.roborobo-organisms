/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/lio_global.h"

#include "activationfunctions/ActivationFunctionPiecewiseLinearTanh.h"

LIOReal ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue = 2; // note: value of 2 corresponds to a rough approx. of tanh

LIOReal ActivationFunctionPiecewiseLinearTanh::apply( LIOReal __value )
{
	// upper / lower bound cases.
	if ( __value <= (LIOReal)0-ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue )
		return (LIOReal)0-ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue;
	else
		if ( __value >= ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue )
			return ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue;

	// linear function btw bounds, results forced in ]-1,+1[  (note, if integer values and _GMaxAbsValue is *definitely fixed* to 2, a simple 1-digit shift would be faster)
	return ( __value / ActivationFunctionPiecewiseLinearTanh::_GMaxAbsValue );
}

//-n
