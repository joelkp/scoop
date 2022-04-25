/* Simple test program for the SCOOP Object module - ExtendedThing module
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

/* This turns SCO_TYPE into an alias for scoExtendedThing when including
 * the first header - Object-ExtendedThing.h - but not otherwise, not even
 * in the nested include of Object-Thing.h.
 */
#define SCO_MODULE scoExtendedThing

#include "Object-ExtendedThing.h"
#include <stdio.h>

void sco_ExtendedThing_do_foo_(scoExtendedThing *o)
{
	printf("do_foo (scoExtendedThing version): %d, %f\n", o->x, o->y);
}

static void scoExtendedThing_virtinit(scoExtendedThing_Meta *o)
{
	scoExtendedThing_Virt *vt = &o->virt;
	vt->do_foo = sco_ExtendedThing_do_foo_;
}
SCO_META(scoExtendedThing, scoThing, 0, scoExtendedThing_virtinit);

SCO_CTOR(scoExtendedThing, sco_ExtendedThing, (scoExtendedThing *o), (o), o)
{
	sco_Thing_ctor((scoThing*)o);
	o->y = 42.f;
	return 1;
}
