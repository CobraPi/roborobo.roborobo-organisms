/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */


#ifndef ACTIVATION_FUNCTION_PIECEWISE_LINEAR_TANH_H
#define ACTIVATION_FUNCTION_PIECEWISE_LINEAR_TANH_H

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"

//#include "activationfunctions/ActivationFunction.h"

/**
 * Picewise approximation of tanh. Centered around zero, linear transfom btw -_GMaxAbsValue and +_GMaxAbsValue. -1 before, +1 above
 * if x <= -_GMaxAbsValue --> -1
 * if x in ]-_GMaxAbsValue,+_GMaxAbsValue[ --> x/_GMaxAbsValue (ie. resulting value btw -1 and 1)
 * if x <= +_GMaxAbsValue --> +1
 *
 * _GMaxAbsValue is set to 2.0 to get a *rough* approximation of tanh(x)
 */
class ActivationFunctionPiecewiseLinearTanh //: public ActivationFunction
{
	private:
		static LIOReal _GMaxAbsValue; // half the range

	protected:

	public:
		//ActivationFunction();
		//virtual ~ActivationFunction() = 0;

		static LIOReal apply( LIOReal __value );
};

#endif // ACTIVATION_FUNCTION_PIECEWISE_LINEAR_TANH_H

//-n
