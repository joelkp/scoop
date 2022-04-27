SCOOP: Simple C library for Object-Oriented Programming
-------------------------------------------------------

SCOOP is a Simple C library for Object-Oriented Programming.
The aim was for it to provide lightweight support for object-oriented APIs
that are structured similarly some older single inheritance C++ codebases,
but in plain C, with minimal verbosity.

This library was made in 2013, as a clean repackaging of experimenting in
2010-2011. A 2022 [article on such object-oriented C](https://joelkp.frama.io/blog/c-struct-inherit.html)
describes the ideas and some basic gotchas to keep in mind about the approach.
Currently, this proof-of-concept library is not actively developed further,
but if of interest further work could go into it. It would probably be more
useful to take a look at the ideas it contains and, given the small size of
it all, to cook up something adapted for your particular project, which may
make use of some elements of SCOOP's approach.

The approach is one of providing some simple routines, along with using the
C preprocessor. Anything that can't be implemented so as to be usable with
great brevity and small overhead is purposefully left out,
including multiple inheritance.

The core of the library is the Object module, which provides support for:
- Class declaration and definition
- Single inheritance (by direct inclusion of the members of the supertype
  into the subtype)
- Virtual functions
- Explicit RTTI checks

See the header files for the library for further details. The library can be built with `make` on many GNU/Linux systems, and with `gmake` on BSDs.

Licensing
---------

SCOOP is distributed under the MIT license below.
The following copyright notice applies to SCOOP as a whole.

Copyright (c) 2010, 2011, 2013, 2022 Joel K. Pettersson.

"Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE."

