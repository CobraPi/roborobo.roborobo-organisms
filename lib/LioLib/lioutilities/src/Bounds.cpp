/*
 *  $Id: Bounds.cpp 2737 2010-09-23 10:19:29Z evert $
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



#ifdef UNITTEST

#include <lioutils/Bounds.h>

#include <test/TestHarness.h>
#include <genetics/RealMutation.h>

//---------------------------------------------------------------------------
// Test case
TEST( Bounds, create )
{
	LIOUtilities::Bounds<int> intBounds;
	CHECK(intBounds.range() == 0);
	intBounds.lower() = 42;
	intBounds.upper() = 1764;
	CHECK(intBounds.lower() == 42);
	CHECK(intBounds.upper() == 1764);
	CHECK(intBounds.range() == (1764-42));

	int foo = 1900;
	CHECK(intBounds.boundHard(foo) == 1764);
	CHECK(foo = 1764);
	foo = 1;
	CHECK(intBounds.boundHard(foo) == 42);
	foo = 99;
	CHECK(intBounds.boundHard(foo) == 99);

	intBounds.lower() = 4;
	intBounds.upper() = 10;
	foo = 12;
	CHECK(intBounds.boundBouncing(foo) == 8);
	foo = -1;
	CHECK(intBounds.boundBouncing(foo) == 9);
	foo = 18;
	CHECK(intBounds.boundBouncing(foo) == 6);
	foo = 9;
	CHECK(intBounds.boundBouncing(foo) == 9);
}
#endif
