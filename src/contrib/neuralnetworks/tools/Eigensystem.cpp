/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#include <neuralnetworks/tools/Eigensystem.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>



namespace Neural {

	gsl_vector_complex* getEigenValues(gsl_matrix* m) {
		gsl_matrix* m2 = gsl_matrix_alloc(m->size1, m->size2);
		gsl_matrix_memcpy(m2, m);

		gsl_vector_complex *eval = gsl_vector_complex_alloc(m2->size1);
		gsl_matrix_complex *evec = gsl_matrix_complex_alloc(m2->size1, m2->size2);

		// Compute Eigenvalues and Eigenvectors
		gsl_eigen_nonsymmv_workspace * w = gsl_eigen_nonsymmv_alloc(m2->size1);
		gsl_eigen_nonsymmv(m2, eval, evec, w);
		gsl_eigen_nonsymmv_free(w);
		gsl_eigen_nonsymmv_sort(eval, evec, GSL_EIGEN_SORT_ABS_DESC);

		gsl_matrix_complex_free(evec);	// Discard eigenvectors
		gsl_matrix_free(m2);
		return eval;
	}

}

