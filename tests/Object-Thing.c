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

/* This turns SCO_TYPE into an alias for scoThing when including the first
 * header - Object-Thing.h - but not otherwise.
 */
#define SCO_MODULE scoThing

#include "Object-Thing.h"
#include <stdio.h>

void sco_Thing_do_foo_(scoThing *o)
{
	printf("do_foo (scoThing version): %d\n", o->x);
}

void sco_Thing_do_bar_(scoThing *o)
{
	puts("do_bar (scoThing version)");
}

static void scoThing_virtinit(scoThing_Meta *o)
{
	scoThing_Virt *vt = &o->virt;
	vt->do_foo = sco_Thing_do_foo_;
	vt->do_bar = sco_Thing_do_bar_;
}
SCO_META(scoThing, scoNone, 0, scoThing_virtinit);

SCO_CTOR(scoThing, sco_Thing, (scoThing *o), (o), o)
{
	o->x = 10;
	return 1;
}
