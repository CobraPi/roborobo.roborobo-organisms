/*
 *  $Id: LIOUtilities.h 2567 2010-07-01 10:14:16Z evert $
 *
 *  Created on: Aug 20, 2009
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

#ifndef LIOUTILITIES_H_
#define LIOUTILITIES_H_

#include <algorithm>

namespace LIOUtilities
{
	/**
	 * Returns -1 for negative numbers, +1 for positive.
	 *
	 * @tparam T Any numeric type
	 */
	template<typename T>
	int sign(T t)
	{
		return (t > 0) ? 1 : -1;
	}

	template <typename T>
	T lowerBound(T value, T min)
	{
		return std::max(value, min);
	}

	template <typename T>
	T upperBound(T value, T max)
	{
		return std::min(value, max);
	}

	template <typename T>
	T limit(T value, T min, T max)
	{
		//assert( min <= max );
		return std::max(std::min(value, max), min);
	}

	template <typename T>
	class Convert{
		public:
			Convert(T orgMin, T orgMax, T targetMin, T targetMax) :
				_orgMin(orgMin),
				_orgMax(orgMax),
				_orgRange(orgMax - orgMin),
				_targetMin(targetMin),
				_targetMax(targetMax),
				_targetRange(targetMax - targetMin)
				{
					assert( _orgMin < _orgMax );
					assert( _targetMin <= _targetMax );
				}

			T operator()(T x) const
			{
				return	_targetMin + (_targetRange * ((x - _orgMin) / _orgRange));
			}
		private:
			T _orgMin;
			T _orgMax;
			T _orgRange;
			T _targetMin;
			T _targetMax;
			T _targetRange;
	};

}

#endif /* LIOUTILITIES_H_ */
