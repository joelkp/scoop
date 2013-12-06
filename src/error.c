/* SCOOP default error functions
 *
 * Copyright (c) 2010, 2011, 2013 Joel K. Pettersson
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

#include <scoop/API.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32

static void default_warning(const char *format, ...)
{
	/* silent, no stderr; can replace with a dialog */
}

static void default_fatal(const char *format, ...)
{
	/* silent, no stderr; can replace with a dialog */
	exit(EXIT_FAILURE);
}

void (*sco_warning)(const char *format, ...) = default_warning;
void (*sco_error)(const char *format, ...) = default_warning;
void (*sco_fatal)(const char *format, ...) = default_fatal;

#else

static void default_warning(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
	fflush(stderr);
}

static void default_fatal(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

void (*sco_warning)(const char *format, ...) = default_warning;
void (*sco_error)(const char *format, ...) = default_warning;
void (*sco_fatal)(const char *format, ...) = default_fatal;

#endif

/* EOF */
