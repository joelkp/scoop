/* Simple test program for the SCOOP Object module - ExtendedThing module
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

#ifndef Object_ExtendedThing_h
#define Object_ExtendedThing_h

#include <scoop/BEGIN.h>
#include "Object-Thing.h"

/*
 * Declare ExtendedThing class. scoExtendedThing_ includes the scoThing_
 * member list (and adds to it), and scoExtendedThing__ includes the
 * scoThing__ virtual function list (in this case, without adding to it).
 */

#define scoExtendedThing_ scoThing_ \
	float y;

#define scoExtendedThing__ scoThing__ \
	void (*do_baz)(SCO_TYPE *o, int value, ...); \

SCOclassdef(scoExtendedThing);

SCOctordec(scoExtendedThing, sco_ExtendedThing,, (SCO_TYPE *o));

/*
 * The scoExtendedThing versions of virtual functions, for direct calls.
 */

void sco_ExtendedThing_do_foo_(SCO_TYPE *o);
void sco_ExtendedThing_do_baz_(SCO_TYPE *o, int string_count, ...);

#include <scoop/END.h>

#endif
