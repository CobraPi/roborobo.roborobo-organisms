/*
 *  $Id: ContextManager.h 3276 2011-02-11 11:00:34Z evert $
 *
 *  Created on: Dec 17, 2010
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

#ifndef CONTEXTMANAGER_H_
#define CONTEXTMANAGER_H_

#include "Context.h"
//#include <genetics/Context.h>
#include <utilities/Logger.h>

#include <functional>
#include <numeric>
#include <iostream>
#include <set>
#include <list>

#include <boost/foreach.hpp>

namespace Genetics
{
/**
 * Maintains list of Context over a run, creates new _contexts as needed.
 */
template<typename real, unsigned dimensions>
class ContextManager
{
public:
	typedef Context<real, dimensions> context;
	typedef std::vector<context> contexts;
	typedef typename contexts::iterator contextIterator;
	typedef typename context::Point Point;

    ContextManager(unsigned minSize = 30, double minProbability = 0.0001) :
    	_currentContext(0),
		_historySize(minSize),
		_minimumProbability(minProbability),
		_log(LIOUtilities::Logger::getCategory("ContextManager"))
	{
		_log.setPriority(log4cpp::Priority::NOTICE);
	}

	/**
	 * Add point \a x to the history. If enough points (i.e. more than \a _historySize)
	 * are in the history, the oldest point in the history is clustered into
	 * the proper context.
	 * Will create a new context from all points in the history if needed.
	 * The current context switches to the (possibly new) context
	 * to which the oldest history point belongs.
	 *
	 * @return the index of the current context. Note that if this is 0, it is
	 * possible that no context has been identified yet.
	 */
	size_t consider(const Point& x)
	{
		history.push_back(x);

		if (history.size() < _historySize)
			return _currentContext;
		// else

		//
		//verify that history is normal distribution, otherwise discard oldest history point and return
		//
		context historyAsContext(history.begin(), history.end());

		BOOST_FOREACH(Point p, history)
		{
			_log.debugStream() << "(x,y): (" << p[0] << ',' << p[1] << ")";
		}

		double p = ksTest(history.begin(), history.end(), historyAsContext);
		if (ksTest(history.begin(), history.end(), historyAsContext) <= 0.1)
		{
			_log.debugStream() << "History not normally distributed (likelihood "<< p << "), discarding oldest entry";

			history.pop_front();
			return _currentContext;
		}

		//
		// Find closest context
		//
		double bestProbability(0.0);

		for (size_t i = 0; i < _contexts.size(); ++i)
		{
			double probability = ksTest(history.begin(), history.end(), _contexts[i]);

			if (probability > bestProbability)
			{
				bestProbability = probability;
				_currentContext = i;
			}
		}

		if (bestProbability < _minimumProbability)
		{
			// create new context from history
			_contexts.push_back(historyAsContext);
			history.clear();
			_currentContext = _contexts.size() - 1;
		} else {
			// add point to best fitting context and remove from history
			_contexts[_currentContext].addPoint(*(history.begin()));
			history.pop_front();
		}

		return _currentContext;
	}

	/**
	 * Prints details of all _contexts on \a os
	 */
	std::ostream& printOn(std::ostream& os)
	{
		BOOST_FOREACH(contextIterator context, _contexts)
		{
			context->printOn(os);
		}

		return os;
	}

    std::vector<context> getContexts() const
    {
        return _contexts;
    }

#ifndef UNITTEST
private:
#endif
    /// Known _contexts
	contexts _contexts;

	/// Last \a _historySize points visited
	std::list<Point> history;

	/// index of the current context in _contexts
	size_t _currentContext;

	/// History size
	unsigned _historySize;

	/// Probability threshold for clustering
	double _minimumProbability;

	log4cpp::Category& _log;

	template<class InputIterator>
	double ksTest(InputIterator begin, InputIterator end, context& context)
	{
		//
		// generate array of standardised differences between points in history and context
		//
		std::vector< Point > standardDiffs;
		size_t p(0);
		for (InputIterator i = begin; i != end; ++i) // loop over history
		{
			Point diff;
			for (unsigned dim = 0; dim < dimensions; ++dim)
			{
				diff[dim] = ((*i)[dim] - context.getMu()[dim]) / context.getSigma()[dim];
			}

			standardDiffs.push_back(diff);
		}

		std::set<real> sortedReducedDifferences;
		BOOST_FOREACH(Point p, standardDiffs)
		{
			//
			// transform standardDiffs to uniform distribution through standard Gaussian CDF
			//
			BOOST_FOREACH(real& diff, p)
			{
				diff = gsl_cdf_ugaussian_P(diff);
			}

			//
			// Reduce dimension by
			// first multiplying the standardised transformed differences across dimensions
			// then gamma CDF over -log of the result
			//
			real product( std::accumulate(p.begin(), p.end(), 1.0, std::multiplies<real>() ));
			sortedReducedDifferences.insert(gsl_cdf_gamma_P(-log(product), dimensions, 1));
		}

		real n( sortedReducedDifferences.size() );
		real stepSize(1/n);
		real step(0.0);
		real KSstatistic(0.0);

		//
		// KS test to compare distributions
		//
		BOOST_FOREACH(double diff, sortedReducedDifferences)
		{
			real delta1 = fabs(step - diff);
			step += stepSize;
			real delta2 = fabs(step - diff);
			KSstatistic = std::max(std::max(delta1, delta2), KSstatistic);
		}

		return KolmogorovProb(sqrt(n)* KSstatistic);
	}

	/**
	 * Adapted from  http://root.cern.ch/root/html/src/TMath.cxx.html#RDBIQ
	 *
	 * Calculates the Kolmogorov distribution function
	 * which gives the probability that Kolmogorov's test statistic will exceed
	 * the value z assuming the null hypothesis. This gives a very powerful
	 * test for comparing two one-dimensional distributions.
	 *  see, for example, Eadie et al, "statistocal Methods in Experimental
	 *  Physics', pp 269-270).
	 *
	 *  This function returns the confidence level for the null hypothesis, where:
	 *   z = dn*sqrt(n), and
	 *   dn  is the maximum deviation between a hypothetical distribution
	 *       function and an experimental distribution with
	 *   n    events
	 *
	 *  NOTE: To compare two experimental distributions with m and n events,
	 *         use z = sqrt(m*n/(m+n))*dn�
	 *
	 * Accuracy: The function is far too accurate for any imaginable application.
	 * Probabilities less than 10^-15 are returned as zero. However, remember that
	 * the formula is only valid for "large" n.
	 *
	 * Theta function inversion formula is used for z <= 1
	 * This function was translated by Rene Brun from PROBKL in CERNLIB.
	 */
	double KolmogorovProb(double z)
	{
		double fj[4] =
		{ -2, -8, -18, -32 }, r[4];
		const double w = 2.50662827;
		// c1 - -pi**2/8, c2 = 9*c1, c3 = 25*c1
		const double c1 = -1.2337005501361697;
		const double c2 = -11.103304951225528;
		const double c3 = -30.842513753404244;

		double u = fabs(z);
		double p;
		if (u < 0.2)
		{
			p = 1;
		}
		else if (u < 0.755)
		{
			double v = 1. / (u * u);
			p = 1 - w * (exp(c1 * v) + exp(c2 * v) + exp(c3 * v)) / u;
		}
		else if (u < 6.8116)
		{
			r[1] = 0;
			r[2] = 0;
			r[3] = 0;
			double v = u * u;
			int maxj = std::max(1, nInt(3. / u));
			for (int j = 0; j < maxj; j++)
			{
				r[j] = exp(fj[j] * v);
			}
			p = 2* (r [0] - r[1] +r[2] - r[3]);
		}
		else
		{
			p = 0;
		}
		return p;
	}

	/**
	 * Adapted from  http://root.cern.ch/root/html/src/TMath.cxx.
	 *
	 * Round to nearest integer. Rounds half integers to the nearest
	 * even integer.
	 */
	int nInt(double x)
	{
	   int i;
	   if (x >= 0) {
	      i = int(x + 0.5);
	      if (x + 0.5 == double(i) && i & 1) i--;
	   } else {
	      i = int(x - 0.5);
	      if (x - 0.5 == double(i) && i & 1) i++;

	   }
	   return i;
	}
};


}

#endif /* CONTEXTMANAGER_H_ */
