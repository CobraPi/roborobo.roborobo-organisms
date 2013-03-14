/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

// nicolas.bredeche(at)lri.fr
// started : 20090127
// revision: ...
//
//
// =-=-=-= SimplePerceptron =-=-=-=
// classic implementation for simple feed-forward perceptron **without** hidden layer.
//
// IMPORTANT NOTICE:
//   - the bias node is **not** included and should be manually provided as an input value (ie. add one input node, then always set to it 1.0)
//   - multiple layered perceptron may be implemented using the eponym class.


#ifndef SIMPLE_PERCEPTRON_H
#define SIMPLE_PERCEPTRON_H

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"

//#include "neuralnets/NeuralNetwork.h"

class SimplePerceptron //: public NeuralNetwork
{
	private:
		int _inputLayerSize; // user must manually include the bias node within the input layer.
		int _outputLayerSize;

		//LIOReal _inputLayerValues[9];
		//LIOReal _outputLayerValues[2];
		//LIOReal _toOutputLayerWeights[18];

		LIOReal *_inputLayerValues;
		LIOReal *_outputLayerValues;
		LIOReal *_toOutputLayerWeights; // input->output

		LIOReal (*applyReadoutActivationFunction)(LIOReal);

	protected:

	public:
		SimplePerceptron( int __inputLayerSize, int __outputLayerSize );
		~SimplePerceptron(void);

		void setActivationFunction( LIOReal (*ptrReadoutActivationFunction)(LIOReal) );

		bool loadParameters(const LIOReal *__weights, int __size );

		void step( LIOReal *__inputValues );

		void getOutputValues( LIOReal *__outputValues );

		 LIOReal* getWeights(void) const
		{ return _toOutputLayerWeights; }

		//size_t serialize(void* buffer) const;
		//void deserialize(void* buffer);

};

#endif // SIMPLE_PERCEPTRON_H


//-n
