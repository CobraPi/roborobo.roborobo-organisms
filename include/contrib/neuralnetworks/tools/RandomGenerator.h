/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H 

#include <gsl/gsl_randist.h>
#include <stdint.h>
#include <time.h>
#include <sys/timeb.h>
#include <sstream>
#include <iostream>


namespace Neural {

	/**
	 * Simple mean to get random double, using gsl
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class RandomGenerator {
		protected:
			gsl_rng *_r;
			gsl_rng *_rgamma;
			gsl_rng *_rgaussian;
			uint64_t const _seed;
			void init();


		public :
			// -+-+-  Constructors/Destructors  -+-+- //
			virtual ~RandomGenerator();
			RandomGenerator();
			RandomGenerator(uint64_t seed);

			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Accessor for seed
			 */
			uint64_t getSeed() const;


			// -+-+-  Main Methods  -+-+- //

			static uint64_t getClockTime();
			double doubleRand();
			double doubleRandGamma(double a, double b);
			double doubleRandGaussian(double sigma);
	};


	extern RandomGenerator rnd;

}

#endif

