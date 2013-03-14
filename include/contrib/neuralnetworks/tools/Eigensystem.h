/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef EIGENSYSTEM_H
#define EIGENSYSTEM_H 

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>


namespace Neural {
	
	/**
	 * @return the eigenvalues of a square matrix
	 */
	gsl_vector_complex* getEigenValues(gsl_matrix* m);

}

#endif


