/*
 *  $Id: Logger.cpp 3037 2011-01-04 11:19:46Z evert $
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

#include <utilities/Logger.h>

#include <log4cpp/OstreamAppender.hh>

#include <ostream>

static const std::string mainCategoryName("liolib");
static const std::string mainAppenderName("standard");
static const log4cpp::Priority::Value defaultPriority(log4cpp::Priority::INFO);

boost::shared_ptr<LIOUtilities::Logger> LIOUtilities::Logger::_instance;

boost::shared_ptr<LIOUtilities::Logger> LIOUtilities::Logger::getInstance(void)
{
	if (!_instance) // first call?
	{
		_instance.reset(new Logger); // create singleton instance
	}

	return _instance;
}

LIOUtilities::Logger::Logger(void) :
	_layout(new log4cpp::PatternLayout())
{
	_layout->setConversionPattern("%d: [%c] %p - %m %n");  // date: [category] priority - message\n

	log4cpp::Appender* appender(new log4cpp::OstreamAppender(mainAppenderName, &std::cout));	// appender to stdout
	appender->setLayout(_layout.get());

	log4cpp::Category& main = log4cpp::Category::getInstance(mainCategoryName);
	main.setAppender(appender);		// appender ownership passes to category!
	main.setPriority(defaultPriority);
}

LIOUtilities::Logger::~Logger(void)
{
	// clean up and flush all appenders
	log4cpp::Category::shutdown();
}

log4cpp::Category& LIOUtilities::Logger::getCategory(void)
{
	LIOUtilities::Logger::getInstance(); // ensure initialisation of log4cpp

	return log4cpp::Category::getInstance(mainCategoryName);
}

log4cpp::Category& LIOUtilities::Logger::getCategory(const std::string& name)
{
	log4cpp::Category& cat = log4cpp::Category::getInstance(name);
	cat.setAppender(*(getCategory().getAppender(mainAppenderName)));
	cat.setPriority(defaultPriority);

	return cat;
}
