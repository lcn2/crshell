/*
 * crshell - start a shell if return is read during a limited a period of time
 *
 * @(#) $Revision: 1.2 $
 * @(#) $Id: crshell.c,v 1.2 2015/09/06 06:45:50 root Exp $
 * @(#) $Source: /usr/local/src/bin/crshell/RCS/crshell.c,v $
 *
 * Copyright (c) 2000 by Landon Curt Noll.  All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright, this permission notice and text
 * this comment, and the disclaimer below appear in all of the following:
 *
 *       supporting documentation
 *       source copies
 *       source works derived from this source
 *       binaries derived from this source or from derived source
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * chongo <was here> /\oo/\
 *
 * Share and enjoy!
 */

#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SHELL "/bin/bash"	/* what we will run when a key is pressed */

static struct termios old;	/* orignal old state of descriptor 0 */
static struct termios new;	/* changed new state of descriptor 0 */
static char *program;		/* our name */

/*
 * restore_state - reset descriptor 0 state
 *
 * given:
 *	fd		- descriptor to restore
 */
static void
restore_state(int fd)
{
    tcsetattr(fd, TCSANOW, &old);
}


/*
 * reset_exit - reset the descriptor 0 state and exit
 *
 * given:
 *	exitval		- exit code by which to exit
 */
static void
reset_exit(int exitval)
{
    restore_state(0);
    exit(exitval);
}


/*
 * sig_catch - catch a signal, reset the terminal an exit code 2
 */
/*ARGSUSED*/
static void
sig_catch(int arg)
{
    reset_exit(2);
}


main(int argc, char **argv)
{
    fd_set in;			/* read selection */
    struct timeval timeout;	/* how long to wait for input */
    int msec;			/* milliseconds to wait */
    char *message;		/* what to initially print without a newline */

    /*
     * parse args
     */
    program = argv[0];
    switch (argc) {
    case 3:
	msec = strtol(argv[1], NULL, 0);
	message = argv[2];
	break;
    default:
    	fprintf(stderr, "usage: %s msec 'message'\n", program);
	exit(1);
    }

    /*
     * obtain terminal state
     */
    if (tcgetattr(0, &old) != 0) {
	perror("tcgetaddr");
	fprintf(stderr, "%s: tcgetattr on 0 failed\n", program);
	exit(3);
    }

    /*
     * setup the new raw terminal state
     */
    new = old;
    new.c_iflag = 0;
    new.c_oflag &= ~OPOST;
    new.c_lflag &= ~(ISIG | ICANON);
    if (tcsetattr(0, TCSANOW, &new) < 0) {
	perror("tcsetaddr");
	fprintf(stderr, "%s: tcsetattr on 0 failed\n", program);
	exit(4);
    }

    /*
     * print message
     */
    printf("%s, press any key for a shell: ", message);
    fflush(stdin);
    fflush(stdout);

    /*
     * firewall - bail on interrupts
     */
    signal(SIGHUP, sig_catch);
    signal(SIGINT, sig_catch);
    signal(SIGQUIT, sig_catch);
    signal(SIGALRM, sig_catch);

    /*
     * wait for something to read
     */
    FD_ZERO(&in);
    FD_SET(0, &in);
    timeout.tv_sec = (long)msec/1000;
    timeout.tv_usec = (long)(msec%1000) * 1000L;
    if (select(1, &in, NULL, NULL, &timeout) > 0) {
	restore_state(0);
	printf("\nstarting a %s shell, type exit to return ...\n\n", SHELL);
	fflush(stdin);
	fflush(stdout);
    	system(SHELL);
	printf("\nreturned from the %s shell");
    }

    /*
     * all done
     */
    restore_state(0);
    printf("\ncontinuing ...\n");
    fflush(stdin);
    fflush(stdout);
    exit(0);
}
