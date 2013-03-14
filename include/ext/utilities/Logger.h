/*
 *  $Id: Logger.h 3038 2011-01-04 11:19:58Z evert $
 *
 *  Created on: Nov 12, 2010
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

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <memory>

#include <boost/shared_ptr.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/PatternLayout.hh>

namespace LIOUtilities
{
/**
 * Singleton log4cpp wrapper class.
 */
class Logger
{
public:
	/**
	 * Retrieve main liolib logging Category. Ensures that log4cpp is initialised to log at default loglevel to stdout
	 */
	static log4cpp::Category& getCategory(void);

	/**
	 * Retrieves named category, making sure it appends to the same stream at the same priority level as the main liolib category.
	 *
	 */
	static log4cpp::Category& getCategory(const std::string& name);

	virtual ~Logger(void);

	/// Singleton access
	static boost::shared_ptr<Logger> getInstance(void);

private:

	// Ctor, copy-ctor and assignment operators private so that instance()
	// is the only entry-point

	Logger(void);

	// Methods below aren't implemented, only declared to prevent accidental copying
	Logger(const Logger&);
	Logger& operator= (const Logger&);


	static boost::shared_ptr<Logger> _instance;

	std::auto_ptr<log4cpp::PatternLayout> _layout;
};

}

#endif /* LOGGER_H_ */
