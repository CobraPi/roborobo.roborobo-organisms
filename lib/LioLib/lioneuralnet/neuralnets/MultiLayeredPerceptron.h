/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

// =-=-=-= MultiLayeredPerceptron =-=-=-=
// classic implementation for fully connected feed-forward multi layered perceptron (one or two hidden layers).
//
// IMPORTANT NOTICE:
//   - the bias node is **not** included and should be manually provided as an input value (ie. add one input node, then always set to it 1.0)
//   - setting the number of hidden node to zero for the first hidden layer is not allowed.


#ifndef MULTI_LAYERED_PERCEPTRON_H
#define MULTI_LAYERED_PERCEPTRON_H

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"

class MultiLayeredPerceptron //: public NeuralNetwork
{
	private:
		int _inputLayerSize; // user must manually include the bias node within the input layer.
		int _outputLayerSize;
		int _hiddenLayerSize;
		int _additionalHiddenLayerSize;

		LIOReal *_inputLayerValues;
		LIOReal *_outputLayerValues;
		LIOReal *_hiddenLayerValues; // first hidden layer
		LIOReal *_additionalHiddenLayerValues; // second hidden layer

		LIOReal *_inputToHiddenLayerWeights; // in->hidden1
		LIOReal *_hiddenLayerToAdditionalHiddenLayerWeights; // hidden1->hidden2
		LIOReal *_toOutputLayerWeights; // hidden1->output OR hidden2->output (depending if there is an additional hidden layer)

		bool _existsAdditionalHiddenLayer;

		LIOReal (*applyActivationFunction)(LIOReal);
		LIOReal (*applyReadoutActivationFunction)(LIOReal);

	protected:

	public:
		//MultiLayeredPerceptron();
		MultiLayeredPerceptron( int __inputLayerSize, int __outputLayerSize, int __hiddenLayerSize, int __additionalHiddenLayerSize = -1);
		virtual ~MultiLayeredPerceptron(void);

		void setActivationFunction( LIOReal (*ptrActivationFunction)(LIOReal) , LIOReal (*ptrReadoutActivationFunction)(LIOReal) );

		bool loadParameters( LIOReal *__weights, int __size );

		void step( LIOReal *__inputValues );

		void getOutputValues( LIOReal *__outputValues );

		//size_t serialize(void* buffer) const;
		//void deserialize(void* buffer);
};

#endif // MULTI_LAYERED_PERCEPTRON_H

//-n
