/*
 *  $Id: Context.h 3276 2011-02-11 11:00:34Z evert $
 *
 *  Created on: Dec 13, 2010
 *      Author: ehaasdi
 *
 * Copyright (c) 2010, VU University Amsterdam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <boost/array.hpp>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include <gsl/gsl_cdf.h>

/**
 *	Describes a context in which an EA population was developed.
 *
 *  @author ehaasdi
 */
namespace Genetics
{

template<typename real, unsigned dimensions>
class Context
{
public:
	typedef boost::array<real, dimensions> Point;

    Context(void) :
		n(0)
	{
		std::fill(mu.begin(), mu.end(), 0);
		std::fill(sumSquares.begin(), sumSquares.end(), 0);
		std::fill(sigma.begin(), sigma.end(), 0);
	}

	template <class InputIterator>
	Context(InputIterator begin, InputIterator end):
		n(0)
	{
		std::fill(mu.begin(), mu.end(), 0);
		std::fill(sumSquares.begin(), sumSquares.end(), 0);
		std::fill(sigma.begin(), sigma.end(), 0);

		for (InputIterator i = begin; i != end; ++i)
			addPoint(*i);
	}

	virtual ~Context()
	{
		// nix
	}

	/**
	 * Adds a point to this cluster, updating mu, sumSquares, sigma and n.
	 * @param x point to add
	 */
	void addPoint(Point& x)
	{
		++n;

		for (unsigned dim = 0; dim < dimensions; ++dim)
		{
			mu[dim] = ((n-1) * mu[dim] + x[dim]) / n;
			sumSquares[dim] += x[dim] * x[dim];

			calculateSigma(dim);
		}
	}

	/**
	 * @return Standardised Euclidean distance (i.e. expressed in standard deviations per
	 * dimension) between x and this context's centre.
	 */
	real distance(const Point& x) const
	{
		real distanceSquared(0);

		for (unsigned dim = 0; dim < dimensions; ++dim)
		{
			distanceSquared += sigma[dim] == 0 ? 0 : (mu[dim] - x[dim]) / sigma[dim];
		}

		return sqrt(distanceSquared);
	}

	std::ostream& printOn(std::ostream& os)
	{
		os << '(';
		for (unsigned dim = 0; dim < dimensions; ++dim)
		{
			os << '(' << mu[dim] << ' ' << sigma[dim] << ") ";
		}
		return os << n << ')';
	}

	unsigned getN(void) const
	{
		return n;
	}

    Point getMu(void) const
    {
        return mu;
    }

    Point getSumSquares(void) const
    {
        return sumSquares;
    }

    Point getSigma(void) const
    {
        return sigma;
    }

    /**
     * Merge other Context into this one, updating statistics
     * @param context
     */
    void absorb(Context<real, dimensions> context)
    {
    	if (context.getN() > 0)
    	{
    		for (unsigned i = 0; i < dimensions; ++i)
    		{
    			mu[i] = ((mu[i] * n) + (context.getMu()[i] * context.getN())) / (n + context.getN());
    			sumSquares[i] += context.getSumSquares()[i];
    		}

    		n += context.getN();

    		for (unsigned i = 0; i < dimensions; ++i)
    		{
    			calculateSigma(i);
    		}
    	}
    }
private:
	Point mu;
	Point sumSquares;
	Point sigma;
	unsigned n;

	double calculateSigma(unsigned dim)
	{
		assert (dim < dimensions);

		if (n > 1)
		{
			sigma[dim] = sqrt((sumSquares[dim] - (mu[dim]*mu[dim]*n)) / (n-1));
		}

		return sigma[dim];
	}
};

}

#endif /* CONTEXT_H_ */
