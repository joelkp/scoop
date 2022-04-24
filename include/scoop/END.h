/* SCOOP convenience header for managing the definition of SCO_TYPE across
 * module header inclusions. This header is included at the end of a module
 * header: at the end of all (or all SCOOP-related) declarations. See the
 * documentation for SCO_TYPE in API.h for details.
 *
 * If a C++ compiler is used, this header also ends an extern "C" block.
 *
 * Copyright (c) 2010, 2013 Joel K. Pettersson
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


/* Work around lack of arithmetic macro redefinition by binary counting... */
#ifdef SCO__0
# undef SCO__0
#elif defined(SCO__1)
# define SCO__0
# undef SCO__1
#elif defined(SCO__2)
# define SCO__0
# define SCO__1
# undef SCO__2
#elif defined(SCO__3)
# define SCO__0
# define SCO__1
# define SCO__2
# undef SCO__3
#elif defined(SCO__4)
# define SCO__0
# define SCO__1
# define SCO__2
# define SCO__3
# undef SCO__4
#elif defined(SCO__5)
# define SCO__0
# define SCO__1
# define SCO__2
# define SCO__3
# define SCO__4
# undef SCO__5
#elif defined(SCO__6)
# define SCO__0
# define SCO__1
# define SCO__2
# define SCO__3
# define SCO__4
# define SCO__5
# undef SCO__6
#elif defined(SCO__7)
# define SCO__0
# define SCO__1
# define SCO__2
# define SCO__3
# define SCO__4
# define SCO__5
# define SCO__6
# undef SCO__7
#else
# undef SCO__FIRST
# undef SCO_TYPE
# define SCO_TYPE SCO_MODULE
# ifdef __cplusplus
}
# endif
#endif
