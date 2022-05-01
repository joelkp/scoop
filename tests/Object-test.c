/* Simple test program for the SCOOP Object module - main module
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

#include "Object-ExtendedThing.h"
#include <stdio.h>

/*
 * Adding an extra type here with no exported symbols...
 */

#define StaticThing_ scoExtendedThing_
#define StaticThing__ scoExtendedThing__
_SCOclassdef(StaticThing);

static void StaticThing_do_bar_(void *o) {
	puts("do_bar (StaticThing version)");
}

static void StaticThing_dtor(StaticThing *o) {
	puts("dtor (StaticThing is soon gone)");
}

static void StaticThing_virtinit(StaticThing_Meta *o)
{
	StaticThing_Virt *vt = &o->virt;
	vt->do_bar = StaticThing_do_bar_; /* overridden */
}

_SCOmetainst(StaticThing, scoExtendedThing,
		StaticThing_dtor, StaticThing_virtinit);
_SCOctordec(StaticThing, StaticThing, (StaticThing *o)); /* optional here */
_SCOctordef(StaticThing, StaticThing, (StaticThing *o), (o), o) {
	sco_ExtendedThing_ctor(o);
	return 1;
}

static StaticThing sthing; /* let's make it global, too */

int main()
{
	scoThing *thing = sco_Thing_new(0);
	scoExtendedThing *ething = sco_ExtendedThing_new(0);
	StaticThing_new(&sthing);

	/* RTTI and virtual function tests.
	 */
	if (sco_of_class(thing, scoThing))
		puts("'thing' is a scoThing");
	thing->meta->virt.do_foo(thing);
	thing->meta->virt.do_bar(thing);

	if (sco_of_class(ething, scoExtendedThing))
		puts("'ething' is a scoExtendedThing");
	if (sco_of_subclass(ething, scoThing))
		puts("'ething' inherits scoThing");
	ething->meta->virt.do_foo(ething);
	ething->meta->virt.do_bar(ething);
	ething->meta->virt.do_baz(ething, 2, "aaa", "bbb");
	sthing.meta->virt.do_bar(&sthing);

	/* Recreate fresh scoThing instance reusing the same memory
	 * allocation.
	 */
	sco_finalize(thing);
	sco_set_metaof(thing, scoThing);
	if (sco_Thing_ctor(thing))
		puts("'thing' reconstructed");

	/* Not necessary as OS cleans up memory at program exit, but included
	 * for testing.
	 */
	sco_delete(thing);
	sco_delete(ething);
	sco_finalize(&sthing); /* this one will print something... */

	return 0;
}
