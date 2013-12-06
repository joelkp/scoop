# SCOOP src dir Makefile
#
# Copyright (c) 2010, 2011, 2013 Joel K. Pettersson
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

MAINDIR		=../
include	../makeinclude
OUTDIR		= $(OBJDIR)
CFLAGS		+= -DSCO_LIBRARY
DSOCFLAGS	+= -DSCO_SHARED

CFILES		= \
		Object.c \
		error.c

LIBNAME		= $(LIBPREFIX)scoop$(LIBSUFFIX)
DSONAME		= $(DSOPREFIX)scoop$(DSOSUFFIX).$(VERSION)
DSOLINK 	= $(DSOPREFIX)scoop$(DSOSUFFIX)
LIBOBJ 		= $(CFILES:.c=.static-o)
DSOOBJ 		= $(CFILES:.c=.shared-o)

# Targets...

all:	$(LIBTARGET) $(DSOTARGET)

static:	$(LIBDIR)$(LIBNAME)

shared:	$(LIBDIR)$(DSONAME)

depend makedepend:
	$(MAKEDEPEND) -I$(INCLUDEDIR) $(CFILES) > makedepend

include makedepend

# Clean...

clean:
	$(RM) $(LIBOBJ) $(DSOOBJ)
	$(RM) $(LIBDIR)$(LIBNAME)
	$(RM) $(LIBDIR)$(DSONAME)

# Linking...

$(LIBDIR)$(LIBNAME): $(LIBOBJ)
	$(RM) $@
	echo "Creating $@..."
	$(LIBCOMMAND) $@ $(LIBOBJ)
	$(RANLIB) $@

$(LIBDIR)$(DSONAME): $(DSOOBJ)
	echo "Linking $@..."
	$(DSOCOMMAND) $@ $(DSOOBJ) $(LIBS)
	$(RM) $(LIBDIR)$(DSOLINK)
	$(LN) $(DSONAME) $(LIBDIR)$(DSOLINK)

# Install/uninstall... (FIXME: to be written)

install:

uninstall:

# EOF
