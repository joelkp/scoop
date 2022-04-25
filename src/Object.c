/* SCOOP Object module
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

#include <scoop/Object.h>
#include <string.h>

static void pure_virtual(void)
{
	sco_fatal("Error: pure virtual SCOOP method called!");
}

/* recursively fills in blank parts of meta type instance chain */
static void init_meta(scoObject_Meta *o)
{
	void (**virt)() = (void (**)()) &o->virt,
			 (**super_virtab)() = 0;
	unsigned int i = 0, max;
	if (o->super) {
		if (!o->super->done)
			init_meta((scoObject_Meta*)o->super);
		super_virtab = (void (**)()) &o->super->virt;
		for (max = o->super->vnum; i < max; ++i)
			if (!virt[i]) virt[i] = super_virtab[i];
	}
	if (o->vtinit)
		o->vtinit(o);
	for (max = o->vnum; i < max; ++i)
		if (!virt[i]) virt[i] = pure_virtual;
	o->done = 1;
}

void* sco_raw_new(void *mem, void *_meta)
{
	scoObject_Meta *meta = _meta;
	if (!mem) {
		if (!(mem = calloc(1, meta->size)))
			return 0;
	} else {
		memset(mem, 0, meta->size);
	}
	if (!meta->done) init_meta(meta);
	sco_set_meta(mem, meta);
	return mem;
}

void sco_delete(void *o)
{
	const scoObject_Meta *meta = sco_meta(o);
	do {
		if (meta->dtor) meta->dtor(o);
		meta = meta->super;
	} while (meta);
	free(o);
}

void sco_finalize(void *o)
{
	const scoObject_Meta *meta = sco_meta(o);
	do {
		if (meta->dtor) meta->dtor(o);
		meta = meta->super;
	} while (meta);
	sco_set_metaof(o, scoNone);
}

int sco_rtticheck(const void *submeta, const void *meta)
{
	const scoObject_Meta *subclass = submeta, *class = meta;
	if (subclass == class)
		return 0;
	do {
		subclass = subclass->super;
		if (subclass == class)
			return 1;
	} while (subclass);
	return -1;
}
