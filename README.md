# crshell

Start a shell if return is read during a limited a period of time.


# To install

```sh
make clobber all
sudo make install clobber
```


# Examples

Prompt and wait 1.5 seconds.  In this example we do not press return:

```sh
$ /usr/local/bin/crshell 1500 'To inspect'
To inspect, press any key for a shell:
continuing ...
```

Prompt and wait 2.5 seconds.  In this example we press return in time.
Once we have the shell, we give the `ls` command and then `exit`.

```sh
$ /usr/local/bin/crshell 2500 Hello
Hello, press any key for a shell: ^M
starting a bash shell, type exit to return ...

$
$ ls
CODE_OF_CONDUCT.md crshell            crshell.o          Makefile           SECURITY.md
CONTRIBUTING.md    crshell.c          LICENSE            README.md
$ exit
exit

returned from the bash shell
continuing ...
```


# To use

```
/usr/local/bin/crshell [-h] [-v level] [-V] [-n] msec message

    -h            print help message and exit
    -v level      set verbosity level (def level: 0)
    -V            print version string and exit

    msec	   timeout in milliseconds
    message	   leading prompt string

Exit codes:
    0         all OK
    2         -h and help string printed or -V and version string printed
    3         command line error
 >= 10        internal error

crshell version: 1.2.1 2025-04-06
```


# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/crshell/security/policy)".
