/*
 * $Id $
 *
 *  Created on: 29 janv. 2009
 *      Author: nicolas.bredeche(at)lri.fr
 */

#ifndef MATHLIB_H
#define MATHLIB_H

#include "lioutils/lio_global.h"

#include "lioutils/Fixed32.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

class Mathlib {

	private:
		// Tanh lookup table - from -8 to 8 (included) with a step size of 0.062745098. Fixed point precision is 8.
		// lookup table generated using LookUpTableGenerator.cpp
		// values should be initialized in Mathlib.cpp. E.g.: "const LIOReal Mathlib::_tanhArray[256] = { -0.99999977 , (...)};"
		static const int _tanhArraySize;
		static const LIOReal _tanhArray[]; // Do not change the accuracy (code assumes 256 discretization values in [-8,+8])


		/// scrambled Hammersley quasi-random sequence (generated off-line with LookUpTableGenerator::generateHammersleyQuasiRandomSequence)
		static const LIOReal _randArray[];

		static const int _randArraySize;
		static int _randSeed;
		static int _randIt;

		// * gaussian random -- (see comments in C file for explanation)
		static int _gaussianRandTick;
		static LIOReal _bufferedGaussianRandValue;

		// This is a typedef for a random number generator.
		// Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
		typedef boost::minstd_rand base_generator_type;
		static base_generator_type _generator;

	public:
		static LIOReal getTanh( LIOReal __value );

		static void setRandSeed( int __seed );

		/**
		 * @return Quasi-random number between 0 and 1 (exclusive). The number is drawn from _randArray.
		 */
		static LIOReal getRand( );
		static LIOReal getGaussianRand( LIOReal m, LIOReal s );

		/**
		 * Toss a biased coin
		 * @param bias The bias of returning \c true
		 * @return true or false
		 *
		 * @author evert
		 */
		static bool getBool(LIOReal bias = 0.5)
		{
			if (bias >= LIOReal(1.0))
				return true;
			// else
			if (bias == LIOReal(0.0))
				return false;
			// else

			return getRand() <= bias;
		}

		/**
		 * @return A random integer value between 0 (inclusive) and \c max (exclusive).
		 */
		static int getInt(int max)
		{
			return int( getRand() * double(max));//(LIOReal) (max + 1)).getMagnitude();
		}
};

#endif // MATHLIB_H

//-n
