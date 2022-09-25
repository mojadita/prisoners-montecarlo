# Makefile -- build script for the prisoners program.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Mon Sep 19 01:26:46 EEST 2022
# Copyright: (c) 2022 Luis Colorado.  All rights reserved.
# License: BSD.

target         = prisoners prisoners.1.gz
toclean        = $(target)

OWN            = -o `id -nu`
GRP            = -g `id -ng`
dirMOD         = -m 0755
exeMOD         = -m 0755
datMOD         = -m 0644
IOPTIONS      ?= $(OWN) $(GRP)

prefix        ?= $(HOME)
bindir        ?= $(prefix)/bin
mandir        ?= $(prefix)/man
man1dir       ?= $(mandir)/man1

INSTALL       ?= install
RM            ?= rm -f
RD            ?= rmdir
GZIP          ?= gzip
GZFLAGS       ?= -v
CFLAGS		  ?= -O3

prisoners_deps =
prisoners_objs = prisoners.o
prisoners_ldlf =
prisoners_libs =
toclean       += $(prisoners_objs)

all: $(target)
clean:
	$(RM) $(toclean)
install: $(target)
	strip prisoners
	$(INSTALL) $(IOPTIONS) $(dirMOD) -d $(bindir)
	$(INSTALL) $(IOPTIONS) $(dirMOD) -d $(mandir)
	$(INSTALL) $(IOPTIONS) $(exeMOD) prisoners $(bindir)/prisoners
	$(INSTALL) $(IOPTIONS) $(datMOD) prisoners.1.gz $(man1dir)/prisoners.1.gz
uninstall:
	$(RM) $(bindir)/prisoners $(man1dir)/prisoners.1.gz
	-$(RD) $(bindir) $(man1dir)

prisoners: $(prisoners_deps) $(prisoners_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) $($@_ldfl) -o $@ \
		$($@_objs) $($@_libs) $(LIBS)
prisoners.1.gz: prisoners.1
	$(GZIP) $(GZFLAGS) <prisoners.1 >$@
