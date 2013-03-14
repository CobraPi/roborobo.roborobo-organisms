/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#ifndef ACTIVATION_FUNCTION_TANH_H
#define ACTIVATION_FUNCTION_TANH_H

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"

//#include "activationfunctions/ActivationFunction.h"

class ActivationFunctionTanh //: public ActivationFunction
{
	private:

	protected:

	public:
		//ActivationFunction();
		//virtual ~ActivationFunction() = 0;

		static LIOReal apply( LIOReal __value );
};

#endif // ACTIVATION_FUNCTION_TANH_H

//-n
