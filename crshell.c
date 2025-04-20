/*
 * crshell - start a shell if return is read during a limited a period of time
 *
 * Copyright (c) 2000,2015,2023,2025 by Landon Curt Noll.  All Rights Reserved.
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
 * chongo (Landon Curt Noll) /\oo/\
 *
 * http://www.isthe.com/chongo/index.html
 * https://github.com/lcn2
 *
 * Share and enjoy!  :-)
 */


#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/errno.h>

#define SHELL "bash"	/* what we will run when a key is pressed */

/*
 * official version
 */
#define VERSION "1.2.1 2025-04-06"          /* format: major.minor YYYY-MM-DD */


/*
 * usage message
 */
static const char * const usage =
  "usage: %s [-h] [-v level] [-V] [-n] msec message\n"
        "\n"
        "    -h            print help message and exit\n"
        "    -v level      set verbosity level (def level: 0)\n"
        "    -V            print version string and exit\n"
        "\n"
        "    msec	   timeout in milliseconds\n"
        "    message	   leading prompt string\n"
        "\n"
        "Exit codes:\n"
        "    0         all OK\n"
        "    2         -h and help string printed or -V and version string printed\n"
        "    3         command line error\n"
        " >= 10        internal error\n"
        "\n"
        "%s version: %s\n";


/*
 * static declarations
 */
static char *program = NULL;    /* our name */
static char *prog = NULL;       /* basename of program */
static const char * const version = VERSION;
static long verbosity = 0;      /* verbosity level */
static struct termios old;	/* original old state of descriptor 0 */
static struct termios new;	/* changed new state of descriptor 0 */
/**/
static void pr_usage(FILE *stream);
static void restore_state(int fd);
static void reset_exit(int exitval);
static void sig_catch(int arg);


/*
 * pr_usage - print usage message
 *
 * given:
 *
 *    stream - print usage message on stream, NULL ==> stderr
 */
static void
pr_usage(FILE *stream)
{
    /*
     * NULL stream means stderr
     */
    if (stream == NULL) {
        stream = stderr;
    }

    /*
     * firewall - change program if NULL
     */
    if (program == NULL) {
        program = "((NULL))";
    }

    /*
     * firewall set name if NULL
     */
    if (prog == NULL) {
        prog = rindex(program, '/');
    }
    /* paranoia if no / is found */
    if (prog == NULL) {
        prog = program;
    } else {
        ++prog;
    }

    /*
     * print usage message to stderr
     */
    fprintf(stream, usage, program, prog, version);
}


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


int
main(int argc, char **argv)
{
    fd_set in;			/* read selection */
    struct timeval timeout;	/* how long to wait for input */
    long msec;			/* milliseconds to wait */
    char *message;		/* what to initially print without a newline */
    int i;

    /*
     * parse args
     */
    program = argv[0];
    while ((i = getopt(argc, argv, ":hv:Vnco:")) != -1) {
        switch (i) {

        case 'h':                   /* -h - print help message and exit */
            pr_usage(stderr);
            exit(2); /* ooo */
            /*NOTREACHED*/

        case 'v':                   /* v level - set verbosity level */
            errno = 0;
            verbosity = strtol(optarg, NULL, 0);
            if (errno != 0 || verbosity < 0) {
                verbosity = 0;
            }
            break;

        case 'V':                   /* -V - print version string and exit */
            (void) printf("%s\n", version);
            exit(2); /* ooo */
            /*NOTREACHED*/

	case ':':
            (void) fprintf(stderr, "%s: ERROR: requires an argument -- %c\n", program, optopt);
            pr_usage(stderr);
            exit(3); /* ooo */
            /*NOTREACHED*/

        case '?':
            (void) fprintf(stderr, "%s: ERROR: illegal option -- %c\n", program, optopt);
            pr_usage(stderr);
            exit(3); /* ooo */
            /*NOTREACHED*/

        default:
            fprintf(stderr, "%s: ERROR: invalid -flag\n", program);
            pr_usage(stderr);
            exit(3); /* ooo */
            /*NOTREACHED*/
        }
    }
    /* skip over command line options */
    argv += optind;
    argc -= optind;
    /* check the arg count */
    switch (argc) {
    case 2:
	msec = strtol(argv[0], NULL, 0);
	message = argv[1];
	break;
    default:
	pr_usage(stderr);
	exit(3);
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
	printf("\nreturned from the %s shell", SHELL);
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
