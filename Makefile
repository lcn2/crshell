#!/usr/bin/env make
#
# crshell - start a shell if return is read during a limited a period of time
#
# Copyright (c) 2000,2023 by Landon Curt Noll.  All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo <was here> /\oo/\
#
# Share and enjoy!


SHELL = bash
DESTBIN = /usr/local/bin
DESTSBIN = /sbin
CC = cc
CFLAGS = -O3 -g3
INSTALL = install

TARGETS = crshell

all: ${TARGETS}

crshell: crshell.c
	${CC} ${CFLAGS} -o crshell crshell.c

install: all
	${INSTALL} -m 0555 ${TARGETS} ${DESTBIN}
	${INSTALL} -m 0555 ${TARGETS} ${DESTSBIN}

clean:
	${RM} *.o

clobber: clean
	${RM} ${TARGETS}
