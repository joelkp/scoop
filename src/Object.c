/* SCOOP Object module
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

#include <scoop/Object.h>
#include <string.h>

/* all pure virtuals are set to this */
static void pure_virtual(void)
{
	sco_fatal("Error: pure virtual SCOOP method called!");
}

/* used to initialize all class descriptions */
static void sco_Object_CD_init(scoObject_CD *o)
{
	void (**vtab)() = (void (**)()) &o->vtab,
			 (**super_vtab)() = 0;
	uint i = 0, max;
	if (o->super) {
		if (!o->super->done)
			sco_Object_CD_init((scoObject_CD*)o->super);
		super_vtab = (void (**)()) &o->super->vtab;
		for (max = o->super->vnum; i < max; ++i)
			if (!vtab[i]) vtab[i] = super_vtab[i];
	}
	if (o->vtinit)
		o->vtinit(o);
	for (max = o->vnum; i < max; ++i)
		if (!vtab[i]) vtab[i] = pure_virtual;
	o->done = 1;
}

void* _sco_new(void *mem, void *classdesc)
{
	scoObject_CD *cd = classdesc;
	if (!mem) {
		if (!(mem = calloc(1, cd->size)))
			return 0;
	} else {
		memset(mem, 0, cd->size);
	}
	if (!cd->done) sco_Object_CD_init(cd);
	sco_set_classdesc(mem, cd);
	return mem;
}

void sco_delete(void *o)
{
	const scoObject_CD *cd = sco_classdesc(o);
	do {
		if (cd->dtor) cd->dtor(o);
		cd = cd->super;
	} while (cd);
	free(o);
}

void sco_finalize(void *o)
{
	const scoObject_CD *cd = sco_classdesc(o);
	do {
		if (cd->dtor) cd->dtor(o);
		cd = cd->super;
	} while (cd);
	sco_set_classdescof(o, scoNull);
}

int _sco_rtticheck(const void *subclassdesc, const void *classdesc)
{
	const scoObject_CD *subclass = subclassdesc, *class = classdesc;
	if (subclass == class) return 0;
	do {
		subclass = subclass->super;
		if (subclass == class) return 1;
	} while (subclass);
	return -1;
}

/* EOF */
