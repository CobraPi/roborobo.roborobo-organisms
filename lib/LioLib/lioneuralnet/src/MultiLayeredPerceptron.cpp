/*
 * $Id $
 *
 *  Created on: 27 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#include "lioutils/lio_global.h"

#include "neuralnets/MultiLayeredPerceptron.h"

#include "activationfunctions/ActivationFunctionPiecewiseLinearTanh.h"
#include "activationfunctions/ActivationFunctionTanh.h"
#include "activationfunctions/ActivationFunctionLinear.h"

/**
 * create a multiple layered perceptron with 1 (or 2) hidden layer(s). Default activation function is hyperbolic tangent for hidden nodes and linear for output nodes.
 */
MultiLayeredPerceptron::MultiLayeredPerceptron( int __inputLayerSize, int __outputLayerSize, int __hiddenLayerSize, int __additionalHiddenLayerSize)
{
	// * initialize activation functions

	applyActivationFunction = &ActivationFunctionPiecewiseLinearTanh::apply;
	applyReadoutActivationFunction = &ActivationFunctionLinear::apply;


	// * initialize neuron value arrays

	_inputLayerSize = __inputLayerSize;
	_outputLayerSize = __outputLayerSize;
	_hiddenLayerSize = __hiddenLayerSize;
	_additionalHiddenLayerSize = __additionalHiddenLayerSize;

	_inputLayerValues = new LIOReal[_inputLayerSize];
	_outputLayerValues = new LIOReal[_outputLayerSize];

	_hiddenLayerValues = new LIOReal[_hiddenLayerSize];

	if ( _additionalHiddenLayerSize == -1 )	// check if additional hidden layer
		_existsAdditionalHiddenLayer = false;
	else
		_existsAdditionalHiddenLayer = true;

	if ( _existsAdditionalHiddenLayer == true )
		_additionalHiddenLayerValues = new LIOReal[_additionalHiddenLayerSize];


	// * initialize weight value arrays

	_inputToHiddenLayerWeights = new LIOReal[ _inputLayerSize * _hiddenLayerSize ];
	if ( _existsAdditionalHiddenLayer == false )
		_toOutputLayerWeights = new LIOReal [ _hiddenLayerSize * _outputLayerSize ];
	else
	{
		_hiddenLayerToAdditionalHiddenLayerWeights = new LIOReal [ _hiddenLayerSize * _additionalHiddenLayerSize ];
		_toOutputLayerWeights = new LIOReal [ _additionalHiddenLayerSize * _outputLayerSize ];
	}
}


MultiLayeredPerceptron::~MultiLayeredPerceptron(void)
{
	delete[] _inputLayerValues;
	delete[] _outputLayerValues;
	delete[] _hiddenLayerValues;
	if ( _existsAdditionalHiddenLayer == true )
		delete[] _additionalHiddenLayerValues;

	delete[] _inputToHiddenLayerWeights;
	if ( _existsAdditionalHiddenLayer == true )
		delete[] _hiddenLayerToAdditionalHiddenLayerWeights;
	delete[] _toOutputLayerWeights;

}

bool MultiLayeredPerceptron::loadParameters( LIOReal *__weights, int __size )
{
	/*
	int checkLengthValue;
	if ( _existsAdditionalHiddenLayer == false )
		checkLengthValue =  _inputLayerSize * _hiddenLayerSize + _hiddenLayerSize * _outputLayerSize;
	else
		checkLengthValue =  _inputLayerSize * _hiddenLayerSize + _hiddenLayerSize * _additionalHiddenLayerSize + _additionalHiddenLayerSize * _outputLayerSize;

	if ( length(__weights) != checkLengthValue )
		return false;
	*/

	for (int i = 0 ; i != __size ; i++ )
	{
		if ( i < _inputLayerSize*_hiddenLayerSize )
			_inputToHiddenLayerWeights[i] = __weights[i];
		else
		{
			if ( _existsAdditionalHiddenLayer == false )
			{
				_toOutputLayerWeights[i-_inputLayerSize*_hiddenLayerSize] = __weights[i];
			}
			else
			{
				if ( i < _inputLayerSize*_hiddenLayerSize + _hiddenLayerSize*_additionalHiddenLayerSize )
					_hiddenLayerToAdditionalHiddenLayerWeights[i - ( _inputLayerSize*_hiddenLayerSize )] = __weights[i];
				else
					_toOutputLayerWeights[i - ( _inputLayerSize*_hiddenLayerSize + _hiddenLayerSize*_additionalHiddenLayerSize )] = __weights[i];
			}
		}
	}

	return true;
}


void MultiLayeredPerceptron::step( LIOReal *__inputValues )
{
	// * preparing network

	for ( int i = 0 ; i != _inputLayerSize ; i++ ) // monitoring.
		_inputLayerValues[i] = __inputValues[i];
	for ( int i = 0 ; i != _hiddenLayerSize ; i++ )
		_hiddenLayerValues[i] = 0;
	if ( _existsAdditionalHiddenLayer == true )
		for ( int i = 0 ; i != _additionalHiddenLayerSize ; i++ )
			_additionalHiddenLayerValues[i] = 0;
	for ( int i = 0 ; i != _outputLayerSize ; i++ )
			_outputLayerValues[i] = 0;

	// * update neuron values

	for ( int i = 0 ; i != _hiddenLayerSize ; i++ )
	{
		for ( int j = 0 ; j != _inputLayerSize ; j++ )
			_hiddenLayerValues[i] += _inputLayerValues[j] * _inputToHiddenLayerWeights[ i*_inputLayerSize + j ];
		_hiddenLayerValues[i] = (*applyActivationFunction)(_hiddenLayerValues[i]);
	}

	if ( _existsAdditionalHiddenLayer == false )
	{
		for ( int i = 0 ; i != _outputLayerSize ; i++ )
		{
			for ( int j = 0 ; j != _hiddenLayerSize ; j++ )
				_outputLayerValues[i] += _hiddenLayerValues[j] * _toOutputLayerWeights[ i*_hiddenLayerSize + j ];
			_outputLayerValues[i] = (*applyReadoutActivationFunction)(_outputLayerValues[i]);
		}
	}
	else
	{
		for ( int i = 0 ; i != _additionalHiddenLayerSize ; i++ )
		{
			for ( int j = 0 ; j != _hiddenLayerSize ; j++ )
				_additionalHiddenLayerValues[i] += _hiddenLayerValues[j] * _hiddenLayerToAdditionalHiddenLayerWeights [ i*_hiddenLayerSize + j ];
			_additionalHiddenLayerValues[i] = (*applyActivationFunction)(_additionalHiddenLayerValues[i]);
		}
		for ( int i = 0 ; i != _outputLayerSize ; i++ )
		{
			for ( int j = 0 ; j != _additionalHiddenLayerSize ; j++ )
				_outputLayerValues[i] += _additionalHiddenLayerValues[j] * _toOutputLayerWeights[ i*_additionalHiddenLayerSize + j ];
			_outputLayerValues[i] = (*applyReadoutActivationFunction)(_outputLayerValues[i]);
		}
	}
}


/**
 * __outputValues must be initialized
 */
void MultiLayeredPerceptron::getOutputValues( LIOReal *__outputValues )
{
	for ( int i = 0 ; i != _outputLayerSize ; i++ )
		__outputValues[i] = _outputLayerValues[i];
	/*
	// test: needs "#include <iostream>"
	std::cout << "values:" << std::endl;
	for ( int i = 0 ; i != _inputLayerSize ; i++ )
		std::cout << _inputLayerValues[i] << std::endl;
	for ( int i = 0 ; i != _hiddenLayerSize ; i++ )
		std::cout << _hiddenLayerValues[i] << std::endl;
	for ( int i = 0 ; i != _outputLayerSize ; i++ )
		std::cout << _outputLayerValues[i] << std::endl;

	std::cout << "weights:" << std::endl;
	for ( int i = 0 ; i != _inputLayerSize*_hiddenLayerSize ; i++ )
		std::cout << "[" << i << "] " << _inputToHiddenLayerWeights[i] << std::endl;
	for ( int i = 0 ; i != _hiddenLayerSize*_outputLayerSize ; i++ )
		std::cout << "[" << i+_inputLayerSize*_hiddenLayerSize << "] " << _toOutputLayerWeights[i] << std::endl;
	*/
}

/**
 * set activation functions (first parameter: hidden nodes activation function ; second parameter: output nodes activation function ("readout") )
 * example of usage: myNeuralNet->setActivationFunction(&ActivationFunctionLinear::apply,&ActivationFunctionLinear::apply);
 */
void MultiLayeredPerceptron::setActivationFunction( LIOReal (*ptrActivationFunction)(LIOReal) , LIOReal (*ptrReadoutActivationFunction)(LIOReal) )
{
	applyActivationFunction = ptrActivationFunction;
	applyReadoutActivationFunction = ptrReadoutActivationFunction;
}

//-n
