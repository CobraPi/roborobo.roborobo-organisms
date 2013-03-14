/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/lio_global.h"

#include "neuralnets/SimplePerceptron.h"

#include "activationfunctions/ActivationFunctionPiecewiseLinearTanh.h"
#include "activationfunctions/ActivationFunctionTanh.h"
#include "activationfunctions/ActivationFunctionSigmoid.h"
#include "activationfunctions/ActivationFunctionLinear.h"

/**
 * create a perceptron with no hidden layer. Default activation function is logistic sigmoid.
 */
SimplePerceptron::SimplePerceptron( int __inputLayerSize, int __outputLayerSize )
{
	// * initialize activation functions

	applyReadoutActivationFunction = &ActivationFunctionTanh::apply;

	// * initialize neuron value arrays

	_inputLayerSize = __inputLayerSize;
	_outputLayerSize = __outputLayerSize;

	_inputLayerValues = new LIOReal[_inputLayerSize];
	_outputLayerValues = new LIOReal[_outputLayerSize];

	_toOutputLayerWeights = new LIOReal [ _inputLayerSize * _outputLayerSize ];
}


SimplePerceptron::~SimplePerceptron(void)
{
	/**/
	delete[] _inputLayerValues;
	delete[] _outputLayerValues;
	delete[] _toOutputLayerWeights;
	/**/
}


bool SimplePerceptron::loadParameters( const LIOReal *__weights, int __size )
{
	for (int i = 0 ; i != __size ; i++ )
		_toOutputLayerWeights[i] = __weights[i];

	return true;
}


void SimplePerceptron::step( LIOReal *__inputValues )
{
	// * preparing network

	for ( int i = 0 ; i != _inputLayerSize ; i++ ) // monitoring.
		_inputLayerValues[i] = __inputValues[i];
	for ( int i = 0 ; i != _outputLayerSize ; i++ )
			_outputLayerValues[i] = 0;

	// * update neuron values

	for ( int i = 0 ; i != _outputLayerSize ; i++ )
	{
		for ( int j = 0 ; j != _inputLayerSize ; j++ ) {
			_outputLayerValues[i] += _inputLayerValues[j] * _toOutputLayerWeights[ i*_inputLayerSize + j ];
		}

		if (applyReadoutActivationFunction) {
			_outputLayerValues[i] = (*applyReadoutActivationFunction)(_outputLayerValues[i]);
		}
	}
}


/**
 * __outputValues must be initialized
 */
void SimplePerceptron::getOutputValues( LIOReal *__outputValues )
{

	 for ( int i = 0 ; i != _outputLayerSize ; i++ )
		__outputValues[i] = _outputLayerValues[i];
	/*
	// test: needs "#include <iostream>"
	std::cout << "values:" << std::endl;
	for ( int i = 0 ; i != _inputLayerSize ; i++ )
		std::cout << _inputLayerValues[i] << std::endl;
	for ( int i = 0 ; i != _outputLayerSize ; i++ )
		std::cout << _outputLayerValues[i] << std::endl;

	std::cout << "weights:" << std::endl;
	for ( int i = 0 ; i != _inputLayerSize*_outputLayerSize ; i++ )
		std::cout << "[" << i << "] " << _toOutputLayerWeights[i] << std::endl;
	*/
}

/**
 * set activation functions (output nodes activation function ("readout") )
 * example of usage: myNeuralNet->setActivationFunction(&ActivationFunctionLinear::apply);
 */
void SimplePerceptron::setActivationFunction( LIOReal (*ptrReadoutActivationFunction)(LIOReal) )
{
	applyReadoutActivationFunction = ptrReadoutActivationFunction;
}

//-n
