/* This file is included by all SCOOP header and source files, and by all
 * code using SCOOP. Typically, it is not included directly but rather
 * through an inclusion of BEGIN.h in a header. (See \ref SCO_TYPE below
 * for more info on BEGIN.h and END.h.)
 *
 * Copyright (c) 2010, 2011, 2013, 2022 Joel K. Pettersson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef SCO_API
# error "scoop/API.h included more than once!"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Common API declarations made available to all source and header files.
 */

#if defined(_WIN32) && !defined(WIN32)
# define WIN32
#endif

#ifdef WIN32
# define _SCO_IMP __declspec(dllimport)
# define _SCO_EXP __declspec(dllexport)
#else
# define _SCO_IMP
# define _SCO_EXP __attribute__((visibility("default")))
#endif

#if !defined(SCO_DOXYGEN) && defined(SCO_SHARED)
# ifdef SCO_LIBRARY
#  define SCO_API  _SCO_EXP
# else
#  define SCO_API  _SCO_IMP
# endif
#else
# define SCO_API
#endif

#if !defined(SCO_USERAPI) || defined(SCO_DOXYGEN)
/**
 * If you use SCOOP within a shared library of your own and define an
 * exported interface, then you'll want to define this according to the
 * platform before including any SCOOP headers, in order for your own SCOOP
 * class declarations (see Object.h) to be exported.
 *
 * SCOOP provides _SCO_EXP, which on Windows becomes "__declspec(dllexport)"
 * and on other systems become "__attribute__((visibility("default")))".
 * This can be convenient to use when building the modules of your
 * shared library. When the headers are referenced from outside your shared
 * library (i.e. by its user), _SCO_IMP can be used as the definition to set
 * it to "__declspec(dllimport)" on Windows, and to make it blank on other
 * platforms.
 *
 * If SCO_USERAPI is undefined when the first SCOOP header is included, it
 * is defined blank.
 */
# define SCO_USERAPI
#endif

/**
 * Used to print a warning message. The default implementation prints the
 * message to stderr, except on Windows (because Windows lacks stderr).
 */
SCO_API extern void (*sco_warning)(const char *msg, ...);

/**
 * Used to print a message when a non-fatal error occurs. The default
 * implementation prints the message to stderr, except on Windows (because
 * Windows lacks stderr).
 */
SCO_API extern void (*sco_error)(const char *msg, ...);

/**
 * Used to print a message when a fatal error occurs. The default
 * implementation prints the message to stderr, except on Windows (because
 * Windows lacks stderr).
 *
 * This function is not allowed to return; an overridden version may however
 * exit it by means of longjmp.
 */
SCO_API extern void (*sco_fatal)(const char *msg, ...);

#if defined(SCO_DEBUG) || defined(SCO_DOXYGEN)
/**
 * If SCO_DEBUG is defined before scoop/API.h is included, SCO_ASSERT will
 * be defined to call sco_fatal() if the condition fails. Otherwise it will
 * expand to a no-op. It is used internally, but can also be used by code
 * using the SCOOP library.
 */
# define SCO_ASSERT(cond) do{ \
  if (!(cond)) sco_fatal("Assertion failed: "#cond); \
}while(0)
#else
# define SCO_ASSERT(cond) ((void)0)
#endif

/*
 * Documentation
 */

/** \macro SCO_MODULE
 See \ref SCO_TYPE for information on what this is for.
*/

/** \macro SCO_TYPE
 SCO_TYPE can be used as the type for object pointers (as in: SCO_TYPE *o)
 in the header for each SCOOP-based module. This can both provide additional
 type safety and remove the need for certain pointer type casts, as follows:

 In a module, define SCO_MODULE as the type the module implements
 (e.g.: flType) before including any SCOOP-based headers. Then SCO_TYPE will
 be set to SCO_MODULE in the first SCOOP-based header included, but to void
 in all the rest - regardless of whether the rest are nested or subsequent
 inclusions.

 If you include the module's header first, then the result is that all of
 its declarations that use SCO_TYPE will refer to the type implemented by
 the module when it is compiled. But declarations in other included module
 headers that use SCO_TYPE will refer to void.
 
 Thus, you can declare the methods for each module's type to use SCO_TYPE
 object pointers. When you compile the module, the methods take pointers to
 objects of the specific type implemented. When you compile another module,
 the methods of the first module take void pointers instead.

 This means:
 - You avoid having to cast from void pointers to the object type you need
   when defining the methods of that type. And you get type-checking, too.
 - You still have the convenience of using void pointers for methods of other
   modules. Both for unrelated types and for supertypes.

 How does it work? A "SCOOP-based header" for a module includes ooc_begin.h
 before it includes other SCOOP-based headers and declares things.
 Following its declarations, it then includes ooc_end.h. (These inclusions
 should be surrounded by an inclusion guard for the header!) The two files
 ooc_begin.h and ooc_end.h implement some ugly hacks using the preprocessor
 that accomplish the deed.

 If you don't need the SCO_MODULE and SCO_TYPE machinery for a module, don't
 worry about it. If you leave SCO_MODULE undefined, SCO_TYPE will always be
 void.
*/

#ifndef SCO_MODULE
# define SCO_MODULE void
#elif defined(SCO_DOXYGEN)
# undef SCO_MODULE
# define SCO_MODULE void
#endif

#ifdef __cplusplus
}
#endif
