/*
 *  $Id: Bounds.h 2738 2010-09-23 10:19:36Z evert $
 *
 *  Created on: Oct 6, 2009
 *      Author: ehaasdi
 *
 * Copyright (c) 2009, VU University Amsterdam
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

#ifndef BOUNDS_H_
#define BOUNDS_H_

#include <utility>

/**
 *
 *  @author ehaasdi
 */
namespace LIOUtilities
{

template <typename T>
class Bounds : public std::pair<T,T>
{
public:

	Bounds(void) :
		std::pair<T,T>(T(0), T(0))
		{}

	Bounds(const T& lower, const T& upper) :
		std::pair<T,T>(lower, upper)
		{}

	const T& lower(void) const { return this->first; }
	const T& upper(void) const { return this->second; }

	T& lower(void) { return this->first; }
	T& upper(void)  { return this->second; }

	T range(void) const { return upper() - lower(); }

	/**
	 * Truncating check against lower-and upper bound.
	 *
	 * @param t reference to value to be checked. May be modified!!
	 * @return t, modified or no
	 */
	T boundHard(T& t) const
	{
		if (range() <= T(0))
			return t;
		// else

		if (t < lower())
			return t = lower();
		// else
		if (t > upper())
			return t = upper();
		// else

		return t;
	}

	/**
	 * Bouncing chack against lower-and upper bound. The resulting value is guaranteed to
	 * be between \c lower() and \c upper() by virtue of bouncing
	 * bounds. Example: if the t is 12 and the upper bound is 10, the value is updated as
	 * follows: 10 - (12 - 10) = 8.
	 *
	 * @param t reference to value to be checked. May be modified!!
	 * @return t, modified or no
	 */
	T& boundBouncing(T& t) const
	{
		T r (range());
		if (r <= T(0))
			return t;
		// else

		if ( t < lower() )
		{
			t = lower() + (lower() - t);
			return boundBouncing(t);
		}
		// else
		if ( t > upper() )
		{
			t = upper() - (t - upper());
			return boundBouncing(t);
		}
		// else

		return t;
	}

	/*
	 * @return True if the bounds are empty, i.e., if lower == upper;
	 */
	bool empty(void) const
	{
		return lower() == upper();
	}
};

}

#endif /* BOUNDS_H_ */
