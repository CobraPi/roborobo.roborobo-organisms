/*
 *  $Id: Context.cpp 3276 2011-02-11 11:00:34Z evert $
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

#include "../include/Context.h"
//#include <genetics/Context.h>

#ifdef UNITTEST

#include <test/TestHarness.h>

using namespace Genetics;

static double values[] = {0.4972,0.4960,0.4474,0.3679,0.4744,0.1769,0.4460,0.4452,0.3729,0.4169,0.4580,0.3773,0.4564,0.4788,0.2590,0.4173,0.2341,0.3464,0.4810,0.3920,0.1988,0.3742,0.3898,0.3835,0.3487,0.3110,0.3271,0.3771,0.4320,0.2782,0.4213,0.3236,0.4533,0.4122,0.2809,0.3994,0.5268,0.0718,0.5193,0.3862,0.5934,0.5160,0.3890,0.3494,0.3850,0.2514,0.1992,0.5399,0.4287,0.4484,0.3145,0.3576,0.3534,0.2606,0.3588,0.3571,0.2370,0.3090,0.3492,0.4073,0.2579,0.3844,0.4489,0.3032,0.3866,0.2557,0.3688,0.3241,0.5879,0.5381,0.2797,0.3752,0.5948,0.4924,0.4164,0.3977,0.3528,0.4242,0.2611,0.4461,0.4243,0.5668,0.2496,0.2741,0.3134,0.3428,0.3906,0.3890,0.2838,0.3942,0.4614,0.3217,0.6161,0.5543,0.6117,0.3651,0.3137,0.4905,0.2611,0.3267};

static double expectedMean(0.3851);
static double expectedSigma(0.1023);
static unsigned n(100);
static double epsilon(0.0005);
//---------------------------------------------------------------------------
// Test case
TEST( Context, addPoint )
{
	typedef Context<double, 2> Context;

	Context context;
	for (unsigned i = 0; i < n; ++i)
	{
		boost::array<double, 2> point;
		point[0] = point[1] = values[i];
		context.addPoint(point);
	}

	CHECK(context.getN() == n);
	CHECK(fabs(context.getMu()[0] - expectedMean) < epsilon);
	CHECK(fabs(context.getSigma()[0] - expectedSigma) < epsilon);
	CHECK(fabs(context.getMu()[1] - expectedMean) < epsilon);
	CHECK(fabs(context.getSigma()[1] - expectedSigma) < epsilon);
}

TEST( Context, ctor )
{

	boost::array<double, 1> points[100];
	for (unsigned i = 0; i < n; ++i)
	{
		points[i][0] = values[i];
	}
	Context<double, 1> context(points, points+n);

	CHECK(context.getN() == n);
	CHECK(fabs(context.getMu()[0] - expectedMean) < epsilon);
	CHECK(fabs(context.getSigma()[0] - expectedSigma) < epsilon);
}


TEST( Context, absorb )
{
	boost::array<double, 1> points[100];
	for (unsigned i = 0; i < n; ++i)
	{
		points[i][0] = values[i];
	}
	unsigned mid = n/2;
	Context<double, 1> context(points, points+mid);
	Context<double, 1> context1(points+ mid, points+n);

	context.absorb(context1);

	CHECK(context.getN() == n);
	CHECK(fabs(context.getMu()[0] - expectedMean) < epsilon);
	CHECK(fabs(context.getSigma()[0] - expectedSigma) < epsilon);
}

#endif
