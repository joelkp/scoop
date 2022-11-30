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
#include <scoop/mempool.h>
#include <stdio.h>

/*
 * Adding an extra type here with no exported symbols...
 */

#define StaticThing_C_ scoExtendedThing_C_
#define StaticThing_V_ scoExtendedThing_V_
SCOclassdef_(StaticThing)

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

SCOmetainst_(StaticThing, scoExtendedThing,
		StaticThing_dtor, StaticThing_virtinit)
SCOctordec_(StaticThing,,, (StaticThing *o)) /* optional here */
SCOctordef_(StaticThing,,, (StaticThing *o), (o)) {
	sco_ExtendedThing_ctor(o);
	return 1;
}

static StaticThing sthing; /* let's make it global, too */

int main()
{
	/* Ordinary dynamic and static allocation.
	 */
	scoThing *thing = sco_Thing_new(0);
	scoExtendedThing *ething = sco_ExtendedThing_new(0);
	StaticThing_new(&sthing);

	/* Allocation using mempool.
	 * Any "mpnew" object lives as long as the mempool.
	 */
	scoMempool *mp = sco_create_Mempool(0);
	scoThing *thing_mp = sco_Thing_mpnew(mp);

	/* RTTI and virtual function tests.
	 */
	if (sco_of_class(thing, scoThing))
		puts("'thing' is a scoThing");
	sco_virt(do_foo, thing);
	sco_virt(do_bar, thing);

	if (sco_of_class(ething, scoExtendedThing))
		puts("'ething' is a scoExtendedThing");
	if (sco_of_subclass(ething, scoThing))
		puts("'ething' inherits scoThing");
	sco_virt(do_foo, ething);
	sco_virt(do_bar, ething);
	sco_virt(do_baz, ething, 2, "aaa", "bbb");
	sco_virt(do_bar, &sthing);

	/* Recreate fresh scoThing instance reusing the same memory
	 * allocation.
	 */
	sco_finalize(thing);
	sco_set_metaof(thing, scoThing);
	if (sco_Thing_ctor(thing))
		puts("'thing' reconstructed");

	/* This would run any associated destructor...
	 */
	sco_destroy_Mempool(mp);

	/* All this clean-up is not strictly necessary as the OS cleans up
	 * memory at program exit, but it's included so as to demonstrate.
	 */
	sco_delete(thing);
	sco_delete(ething);
	sco_finalize(&sthing); /* this one will print something... */

	return 0;
}
