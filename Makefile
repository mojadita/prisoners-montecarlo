# Makefile -- build script for the prisoners program.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Mon Sep 19 01:26:46 EEST 2022
# Copyright: (c) 2022 Luis Colorado.  All rights reserved.
# License: BSD.

target         = prisoners
toclean        = $(target)

RM            ?= rm -f

prisoners_deps =
prisoners_objs = prisoners.o
prisoners_ldlf =
prisoners_libs =
toclean       += $(prisoners_objs)

all: $(target)
clean:
	$(RM) $(toclean)

prisoners: $(prisoners_deps) $(prisoners_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) $($@_ldfl) -o $@ \
		$($@_objs) $($@_libs) $(LIBS)
