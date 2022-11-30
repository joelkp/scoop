/* SCOOP memory pool module (originally from saugns)
 *
 * Copyright (c) 2014, 2018-2022 Joel K. Pettersson
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

#ifndef scoop_mempool_h
#define scoop_mempool_h
#ifndef SCO_API
# include "API.h"
#endif
#include <stddef.h>
#ifdef __cplusplus
# define restrict /* nothing for now */
extern "C" {
#endif

struct scoMempool;
typedef struct scoMempool scoMempool;

scoMempool *sco_create_Mempool(size_t start_size) scoMalloclike;
void sco_destroy_Mempool(scoMempool *restrict o);

void *sco_mpalloc(scoMempool *restrict o, size_t size) scoMalloclike;
void *sco_mpmemdup(scoMempool *restrict o,
		const void *restrict src, size_t size) scoMalloclike;
#ifndef SCO__DTOR_F_DEFINED
# define SCO__DTOR_F_DEFINED
typedef void (*scoDtor_f)(void *o);
#endif
scoFakeBool sco_mpregdtor(scoMempool *restrict o,
		scoDtor_f func, void *restrict arg);

#ifdef __cplusplus
}
# undef restrict
#endif
#endif
