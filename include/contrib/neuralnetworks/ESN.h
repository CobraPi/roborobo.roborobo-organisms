/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */
// TODO : Support for biais neurons ?

#ifndef ESN_H
#define ESN_H 

#include <neuralnetworks/ReservoirNeuralNetwork.h>
#include <string>
#include <vector>
#include <gsl/gsl_matrix.h>

#include "neuralnetworks/tools/RandomGenerator.h"
#include "RoboroboMain/roborobo.h"


namespace Neural {

	/**
	 * A support for ESN neural networks
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class ESN : public ReservoirNeuralNetwork {

		protected:

			/** Weights Matrix */
			gsl_matrix* _w;

			/** Values of each neurons */
			gsl_vector* _neuronsValues;

			/** New values of each neurons */
			gsl_vector* _newValues;

			/** Density of connections */
			double _densityOfConnections;

			/** Reservoir's alpha parameter */
			double _alpha;

			/* Matrices/vectors views */
			gsl_vector_view _neuronsValuesInputs;
			gsl_vector_view _neuronsValuesReservoir;
			gsl_vector_view _neuronsValuesOutputs;
			gsl_matrix_view _wInputsReservoir;
			gsl_matrix_view _wReservoirReservoir;
			gsl_matrix_view _wOutputsReservoir;
			gsl_vector_view _newValuesReservoir;
			gsl_vector_view _newValuesOutput;
        
			/**
			 * Create base stuffs of the ESN
			 */
			void createESN();




		public:
			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights();

			// -+-+-  Constructors/Destructors  -+-+- //

			ESN(std::vector<double>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize,
					double densityOfConnections, double alpha,
					double inputToReservoirWeightRange,
					double inputToOutputWeightRange,
					double reservoirToOutputWeightRange,
					double outputToReservoirWeightRange,
					double outputToOutputWeightRange,
					bool allowInputToOutputDirectConnections,
					bool allowOutputSelfRecurrentConnections,
					bool allowInputToReservoirConnections,
					bool allowOutputToReservoirConnections);

			ESN(size_t nbInputs, size_t nbOutputs, size_t reservoirSize,
					double densityOfConnections, double alpha,
					double inputToReservoirWeightRange,
					double inputToOutputWeightRange,
					double reservoirToOutputWeightRange,
					double outputToReservoirWeightRange,
					double outputToOutputWeightRange,
					bool allowInputToOutputDirectConnections,
					bool allowOutputSelfRecurrentConnections,
					bool allowInputToReservoirConnections,
					bool allowOutputToReservoirConnections);

			ESN(std::vector<double>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize,
					double densityOfConnections, double alpha);

			ESN(size_t nbInputs, size_t nbOutputs, size_t reservoirSize, double densityOfConnections, double alpha);

			/** Deep Copy constructor */
			ESN(ESN const& other);

			virtual ~ESN();


			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Accessor for the neurons values
			 */
			const gsl_vector& getNeuronsValues() const;

			/**
			 * Accessor for densityOfConnections
			 */
			unsigned int getDensityOfConnections() const;

			/**
			 * Accessor for alpha
			 */
			unsigned int getAlpha() const;


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual ESN* clone() const;

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const;

			/**
			 * {@InheritDoc}
			 */
			virtual void step();

			/**
			 * Return a string identifying this class
			 */
			static std::string getNNTypeName();

			/**
			 * @return the id of the first input neuron in the weights matrix and in neuronsValues vector
			 */
			size_t getStartInput();

			/**
			 * @return the id of the first reservoir neuron in the weights matrix and in neuronsValues vector
			 */
			size_t getStartReservoir();

			/**
			 * @return the id of the first output neuron in the weights matrix and in neuronsValues vector
			 */
			size_t getStartOutput();

	};

}


#endif

