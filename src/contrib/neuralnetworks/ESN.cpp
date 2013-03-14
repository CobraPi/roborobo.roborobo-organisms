/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#include <neuralnetworks/ESN.h>
#include <neuralnetworks/NeuralNetwork.h>
#include <sstream>
#include <memory>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>


using namespace Neural;


#include <neuralnetworks/tools/RandomGenerator.h>
#include <neuralnetworks/tools/Eigensystem.h>



/* --------------------- ESN --------------------- */

ESN::ESN(std::vector<double>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize, double densityOfConnections, double alpha,
		double inputToReservoirWeightRange,
		double inputToOutputWeightRange,
		double reservoirToOutputWeightRange,
		double outputToReservoirWeightRange,
		double outputToOutputWeightRange,
		bool allowInputToOutputDirectConnections,
		bool allowOutputSelfRecurrentConnections,
		bool allowInputToReservoirConnections,
		bool allowOutputToReservoirConnections) :
		ReservoirNeuralNetwork(weights, nbInputs, nbOutputs, reservoirSize, inputToReservoirWeightRange, inputToOutputWeightRange, reservoirToOutputWeightRange, outputToReservoirWeightRange, outputToOutputWeightRange, allowInputToOutputDirectConnections, allowOutputSelfRecurrentConnections, allowInputToReservoirConnections, allowOutputToReservoirConnections), _densityOfConnections(densityOfConnections), _alpha(alpha) {

	createESN();
}

ESN::ESN(size_t nbInputs, size_t nbOutputs, size_t reservoirSize, double densityOfConnections, double alpha,
		double inputToReservoirWeightRange,
		double inputToOutputWeightRange,
		double reservoirToOutputWeightRange,
		double outputToReservoirWeightRange,
		double outputToOutputWeightRange,
		bool allowInputToOutputDirectConnections,
		bool allowOutputSelfRecurrentConnections,
		bool allowInputToReservoirConnections,
		bool allowOutputToReservoirConnections) :
		ReservoirNeuralNetwork(nbInputs, nbOutputs, reservoirSize, inputToReservoirWeightRange, inputToOutputWeightRange, reservoirToOutputWeightRange, outputToReservoirWeightRange, outputToOutputWeightRange, allowInputToOutputDirectConnections, allowOutputSelfRecurrentConnections, allowInputToReservoirConnections, allowOutputToReservoirConnections), _densityOfConnections(densityOfConnections), _alpha(alpha) {

	createESN();
}

ESN::ESN(std::vector<double>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize,
		double densityOfConnections, double alpha) :
		ReservoirNeuralNetwork(weights, nbInputs, nbOutputs, reservoirSize), _densityOfConnections(densityOfConnections), _alpha(alpha) {
	createESN();
}

ESN::ESN(size_t nbInputs, size_t nbOutputs, size_t reservoirSize,
		double densityOfConnections, double alpha) :
		ReservoirNeuralNetwork(nbInputs, nbOutputs, reservoirSize), _densityOfConnections(densityOfConnections), _alpha(alpha) {
	createESN();
}

ESN::ESN(ESN const& other) : ReservoirNeuralNetwork(other) {
	_densityOfConnections = other._densityOfConnections;
	_alpha = other._alpha;

	size_t const nbNeurons = getNbNeurons();
	_w = gsl_matrix_alloc(nbNeurons, nbNeurons);
	gsl_matrix_memcpy(_w, other._w);

	_neuronsValues = gsl_vector_alloc(nbNeurons);
	gsl_vector_memcpy(_neuronsValues, other._neuronsValues);

	_newValues = gsl_vector_alloc(nbNeurons);
	gsl_vector_memcpy(_newValues, other._newValues);

	_neuronsValuesInputs = gsl_vector_subvector(_neuronsValues, getStartInput(), _nbInputs);
	_neuronsValuesReservoir = gsl_vector_subvector(_neuronsValues, getStartReservoir(), _reservoirSize);
	_neuronsValuesOutputs = gsl_vector_subvector(_neuronsValues, getStartOutput(), _nbOutputs);
	_wInputsReservoir = gsl_matrix_submatrix(_w, getStartInput(), getStartReservoir(), _nbInputs, _reservoirSize);
	_wReservoirReservoir = gsl_matrix_submatrix(_w, getStartReservoir(), getStartReservoir(), _reservoirSize, _reservoirSize);
	_wOutputsReservoir = gsl_matrix_submatrix(_w, getStartOutput(), getStartReservoir(), _nbOutputs, _reservoirSize);
	_newValuesReservoir = gsl_vector_subvector(_newValues, getStartReservoir(), _reservoirSize);
	_newValuesOutput = gsl_vector_subvector(_newValues, getStartOutput(), _nbOutputs);
}

ESN::~ESN() {
	gsl_matrix_free(_w);
	gsl_vector_free(_neuronsValues);
	gsl_vector_free(_newValues);
}


std::string ESN::toString() const {
	return ReservoirNeuralNetwork::toString();
}



void ESN::createESN() {

	// Define some constants, for readability
	size_t const nbNeurons = getNbNeurons();
	size_t const startInput = getStartInput();
	size_t const startReservoir = getStartReservoir();
	size_t const startOutput = getStartOutput();

	// Create weights matrix
	_w = gsl_matrix_alloc(nbNeurons, nbNeurons);

	// Fill the matrix with zeroes (should be done !)
	for (size_t i = 0; i < _w->size1; i++)
		for (size_t j = 0; j < _w->size2; j++)
			gsl_matrix_set(_w, i, j, 0);

	// Create neurons values
	_neuronsValues = gsl_vector_alloc(nbNeurons);
	_newValues = gsl_vector_alloc(nbNeurons);

	// Fill the matrices with zeroes (should be done !)
	for (size_t i = 0; i < _neuronsValues->size; i++)
		gsl_vector_set(_neuronsValues, i, 0);
	for (size_t i = 0; i < _newValues->size; i++)
		gsl_vector_set(_newValues, i, 0);

	// Create inputs to inputs : always 0
	for(size_t i = startInput; i < startInput + _nbInputs; i++)
		for(size_t j = startInput; j < startInput + _nbInputs; j++)
			gsl_matrix_set(_w, i, j, 0);

	// Create inputs to reservoir, if allowed
	if(_allowInputToReservoirConnections) {
		for(size_t i = startInput; i < startInput + _nbInputs; i++)
			for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++)
				gsl_matrix_set(_w, i, j, rnd.doubleRand() * 2.0 * _inputToReservoirWeightRange - _inputToReservoirWeightRange);
	} else {
		for(size_t i = startInput; i < startInput + _nbInputs; i++)
			for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++)
				gsl_matrix_set(_w, i, j, 0);
	}

	// Create inputs to outputs, if allowed
	if(_allowInputToOutputDirectConnections) {
		for(size_t i = startInput; i < startInput + _nbInputs; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				gsl_matrix_set(_w, i, j, rnd.doubleRand() * 2.0 * _inputToOutputWeightRange - _inputToOutputWeightRange);
	} else {
		for(size_t i = startInput; i < startInput + _nbInputs; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				gsl_matrix_set(_w, i, j, 0);
	}

	// Create outputs self recurrent connections, if allowed
	if(_allowOutputSelfRecurrentConnections) {
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				gsl_matrix_set(_w, i, j, rnd.doubleRand() * 2.0 * _outputToOutputWeightRange - _outputToOutputWeightRange);
	} else {
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				gsl_matrix_set(_w, i, j, 0);
	}
//	for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
//		for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
//			gsl_matrix_set(_w, i, j, 0);
//	if(_allowOutputSelfRecurrentConnections) {
//		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
//			gsl_matrix_set(_w, i, i, rnd.doubleRand() * 2.0 * _outputToOutputWeightRange - _outputToOutputWeightRange);
//	}

	// Create outputs to reservoir, if allowed
	if(_allowOutputToReservoirConnections) {
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++)
				gsl_matrix_set(_w, i, j, rnd.doubleRand() * 2.0 * _outputToReservoirWeightRange - _outputToReservoirWeightRange);
	} else {
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++)
				gsl_matrix_set(_w, i, j, 0);
	}

	// --- Create reservoir ---
	// Build connectivity matrix
	for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
		for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++) {
			if(rnd.doubleRand() < _densityOfConnections) {
				if(rnd.doubleRand() < 0.5)
					gsl_matrix_set(_w, i, j, 1);
				else
					gsl_matrix_set(_w, i, j, -1);
			} else
				gsl_matrix_set(_w, i, j, 0);
		}

	gsl_matrix_view matReservoirView = gsl_matrix_submatrix(_w, startReservoir, startReservoir, _reservoirSize, _reservoirSize);
	gsl_matrix matReservoir = matReservoirView.matrix;

	// Normalize Matrix wrt spectral radius
	gsl_vector_complex* eval = getEigenValues(&matReservoir);
	gsl_vector* spectrum = gsl_vector_alloc(nbNeurons);
	for(size_t i = 0; i < _reservoirSize; i++) {
		gsl_complex complex_val = gsl_vector_complex_get(eval, i);
		double spectrum_val = sqrt(GSL_REAL(complex_val) * GSL_REAL(complex_val) +
				GSL_IMAG(complex_val) * GSL_IMAG(complex_val));
		gsl_vector_set(spectrum, i, spectrum_val);
	}
	gsl_vector_complex_free(eval);

	// Get spectral radius
	double spectralRadius = gsl_vector_get(spectrum, 0);
	for(size_t i = 1; i < _reservoirSize; i++)
		if(spectralRadius < gsl_vector_get(spectrum, i))
			spectralRadius = gsl_vector_get(spectrum, i);

	// Free spectum
	gsl_vector_free(spectrum);

	if(spectralRadius == 0)
		throw "Spectral radius for ESN cannot be 0. Setting higher density may help.";

	// Scale and damp weights
	for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
		for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++) {
			double prev = gsl_matrix_get(_w, i, j);
			gsl_matrix_set(_w, i, j, prev / spectralRadius * _alpha);
		}

	// Create the matrices/vectors views
	_neuronsValuesInputs = gsl_vector_subvector(_neuronsValues, getStartInput(), _nbInputs);
	_neuronsValuesReservoir = gsl_vector_subvector(_neuronsValues, getStartReservoir(), _reservoirSize);
	_neuronsValuesOutputs = gsl_vector_subvector(_neuronsValues, getStartOutput(), _nbOutputs);
	_wInputsReservoir = gsl_matrix_submatrix(_w, getStartInput(), getStartReservoir(), _nbInputs, _reservoirSize);
	_wReservoirReservoir = gsl_matrix_submatrix(_w, getStartReservoir(), getStartReservoir(), _reservoirSize, _reservoirSize);
	_wOutputsReservoir = gsl_matrix_submatrix(_w, getStartOutput(), getStartReservoir(), _nbOutputs, _reservoirSize);
	_newValuesReservoir = gsl_vector_subvector(_newValues, getStartReservoir(), _reservoirSize);
	_newValuesOutput = gsl_vector_subvector(_newValues, getStartOutput(), _nbOutputs);
}


void ESN::step() {
	size_t weightsIndice = 0;

	// Fill the vector with zeroes (should be done !)
	for (size_t i = 0; i < getNbNeurons(); i++)
		gsl_vector_set(_newValues, i, 0.0);

	// Put inputs values into neuronsValues
	for(size_t i = 0; i < _nbInputs; i++)
		gsl_vector_set(_neuronsValues, getStartInput() + i, _inputs[i]);

	// Put outputs values into neuronsValues
	for(size_t i = 0; i < _nbOutputs; i++)
		gsl_vector_set(_neuronsValues, getStartOutput() + i, _outputs[i]);

	// Compute new values of the reservoir
	if(_allowInputToReservoirConnections)
		gsl_blas_dgemv(CblasTrans, 1.0, &_wInputsReservoir.matrix, &_neuronsValuesInputs.vector, 1.0, &_newValuesReservoir.vector);
	gsl_blas_dgemv(CblasTrans, 1.0, &_wReservoirReservoir.matrix, &_neuronsValuesReservoir.vector, 1.0, &_newValuesReservoir.vector);
	if(_allowOutputToReservoirConnections)
		gsl_blas_dgemv(CblasTrans, 1.0, &_wOutputsReservoir.matrix, &_neuronsValuesOutputs.vector, 1.0, &_newValuesReservoir.vector);

	// Update reservoir
	for(size_t j = getStartReservoir(); j < getStartReservoir() + _reservoirSize; j++) {
		double foo = tanh(gsl_vector_get(_newValues, j));
		gsl_vector_set(_neuronsValues, j, foo);
		gsl_vector_set(_newValues, j, foo);
	}

	// Create an array from the weights
	size_t nbWeights = getRequiredNumberOfWeights();
	double weightsArray[nbWeights];
	for(size_t i = 0; i < nbWeights; i++)
		weightsArray[i] = _weights[i];

	// Compute new values of outputs
	if(_allowInputToOutputDirectConnections) {
		gsl_matrix_view weightsInputs = gsl_matrix_view_array((double*)(weightsArray + weightsIndice), _nbInputs, _nbOutputs);
		gsl_blas_dgemv(CblasTrans, 1.0, &weightsInputs.matrix, &_neuronsValuesInputs.vector, 1.0, &_newValuesOutput.vector);
		weightsIndice += _nbInputs * _nbOutputs;
	}
	gsl_matrix_view weightsReservoir = gsl_matrix_view_array((double*)(weightsArray + weightsIndice), _reservoirSize, _nbOutputs);
	gsl_blas_dgemv(CblasTrans, 1.0, &weightsReservoir.matrix, &_neuronsValuesReservoir.vector, 1.0, &_newValuesOutput.vector);
	weightsIndice += _reservoirSize * _nbOutputs;
	if(_allowOutputSelfRecurrentConnections) {
		gsl_matrix_view weightsOutputs = gsl_matrix_view_array((double*)(weightsArray + weightsIndice), _nbOutputs, _nbOutputs);
		gsl_blas_dgemv(CblasTrans, 1.0, &weightsOutputs.matrix, &_neuronsValuesOutputs.vector, 1.0, &_newValuesOutput.vector);
		weightsIndice += _nbOutputs * _nbOutputs;
	}
	
	// Update outputs
	for(size_t j = getStartOutput(); j < getStartOutput() + _nbOutputs; j++) {
		double foo = tanh(gsl_vector_get(_newValues, j));
//		if((foo <= 1.0 && foo >= 0.9999) || (foo >= -1.0 && foo <= -0.9999)) // XXX : DEBUG
//			std::cout << "AAAAAAAAAAAAA : " << gsl_vector_get(_newValues, j) << " " << foo << std::endl;
		gsl_vector_set(_neuronsValues, j, foo);
//		gsl_vector_set(_newValues, j, foo);
	}

	// Update outputs values
	for(size_t i = 0; i < _nbOutputs; i++)
		_outputs[i] = gsl_vector_get(_neuronsValues, getStartOutput() + i);
}


unsigned int ESN::computeRequiredNumberOfWeights() {
	size_t result = 0;
	result += _reservoirSize * _nbOutputs; // Reservoir --> output connexions

	if(_allowInputToOutputDirectConnections)
		result += _nbInputs * _nbOutputs;
	if(_allowOutputSelfRecurrentConnections)
		result += _nbOutputs * _nbOutputs;

	return result;
}

std::string ESN::getNNTypeName() {
	return "ESN";
}

unsigned int ESN::getDensityOfConnections() const {
	return _densityOfConnections;
}

unsigned int ESN::getAlpha() const {
	return _alpha;
}

const gsl_vector& ESN::getNeuronsValues() const {
	return *_neuronsValues;
}

size_t ESN::getStartInput() {
	return 0;
}

size_t ESN::getStartReservoir() {
	return getStartInput() + _nbInputs;
}

size_t ESN::getStartOutput() {
	return getStartReservoir() + _reservoirSize;
}

ESN* ESN::clone() const {
	return new ESN(*this);
}

