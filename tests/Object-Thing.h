/* Simple test program for the SCOOP Object module - Thing module
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

#ifndef Object_Thing_h
#define Object_Thing_h

#include <scoop/BEGIN.h>
#include <scoop/Object.h>

/*
 * Declare Thing class. scoThing_ lists the class members (minus the class
 * description which is always included), and scoThing__ lists the virtual
 * functions.
 */

#define scoThing_ \
  int x;

#define scoThing__ \
  void (*do_foo)(SCO_TYPE *o); \
  void (*do_bar)(SCO_TYPE *o);

SCOclass(scoThing);

SCOclassctor(scoThing, sco_Thing, (SCO_TYPE *o));

/*
 * Macros to make virtual functions look like plain functions.
 * Naming convention is limited: These are named after the first class
 * in which the virtual function in question was defined.
 */

#define sco_Thing_do_foo(o) \
	(o)->classdesc->vtab.do_foo(o)

#define sco_Thing_do_bar(o) \
	(o)->classdesc->vtab.do_bar(o)

/*
 * The scoThing versions of virtual functions, for direct calls:
 */

void sco_Thing_do_foo_(SCO_TYPE *o);
void sco_Thing_do_bar_(SCO_TYPE *o);


#include <scoop/END.h>

#endif
/* EOF */
