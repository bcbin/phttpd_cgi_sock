#include "unp.h"

void catch_child(int i)
{
    pid_t pid;
    int status;

    pid = wait(&status);
    fprintf(stderr, "parent: child process pid=%d exited with value %d\n",
            pid, WEXITSTATUS(status));
}

void catch_interrupt(int i)
{
    fprintf(stderr, "catch interrupt");
    exit(EXIT_SUCCESS);
}

void signal_init()
{
    /* detect child termination */
    //signal(SIGCHLD, catch_child);

    /* ctrl callback */
    signal(SIGINT, catch_interrupt);
}
