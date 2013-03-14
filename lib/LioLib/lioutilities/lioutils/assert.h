/*
 *  $Id: assert.h  $
 *
 *  Created on: 12-Feb-2009
 *      Author: ehaasdi
 *
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef VERBOSE_EMBEDDED

/* Allow this file to be included multiple times
   with different settings of NDEBUG.  */
#undef assert

#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else

#include <core/SysLog.h>

/**
 * Exactly the same as the standard C assert function, but on the Cortex.
 */
#define assert(expression)  \
  ((void) ((expression) ? 0 : (SYSLOG << " : failed assertion " << #expression << '\n';)))

#endif // NDEBUG

#else // Standard IO ops available, standard assert will do just fine

#include <assert.h>

#endif


#endif /* ASSERT_H_ */
