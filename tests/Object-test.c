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

int main()
{
	scoThing *thing = sco_Thing_new(0);
	scoExtendedThing *ething = sco_ExtendedThing_new(0);

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

	return 0;
}
