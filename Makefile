#!/usr/bin/make
#
# crshell - start a shell if return is read during a limited a period of time
#
# @(#) $Revision$
# @(#) $Id$
# @(#) $Source$
#
# Copyright (c) 2000 by Landon Curt Noll.  All Rights Reserved.
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


SHELL = /bin/sh
DEST = /usr/local/bin
RM = /bin/rm -f
CP = /bin/cp
CHMOD = /bin/chmod
CC = cc
CFLAGS = -O2

TARGETS = crshell

all: ${TARGETS}

crshell: crshell.c
	${CC} ${CFLAGS} -o crshell crshell.c

install: all
	@for i in ${TARGETS}; do \
	    echo "${RM} ${DEST}/$$i"; \
	    ${RM} ${DEST}/$$i; \
	    echo "${CP} $$i ${DEST}"; \
	    ${CP} $$i ${DEST}; \
	    echo "${CHMOD} 0555 ${DEST}/$$i"; \
	    ${CHMOD} 0555 ${DEST}/$$i; \
	done

clean:
	${RM} *.o

clobber: clean
	${RM} ${TARGETS}
