#include "unp.h"

void Write(int fd, void *ptr, size_t nbytes)
{
    if(write(fd, ptr, nbytes) != nbytes) {
        perror("[ERROR] Unix - write error.\n");
        exit(0);
    }
}

int Open(const char *pathname, int oflag, mode_t mode)
{
    int     fd;

    if ( (fd = open(pathname, oflag, mode)) == -1) {
        perror("[ERROR] Unix - open error.\n");
        exit(0);
    }

    return(fd);
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t     n;

    if ( (n = read(fd, ptr, nbytes)) == -1) {
        perror("[ERROR] Unix - read error.\n");
        exit(0);
    }
    return(n);
}

void Close(int fd)
{
    if (close(fd) == -1) {
        perror("[ERROR] Unix - close error.\n");
        exit(0);
    }
}

void * Malloc(size_t size)
{
    void    *ptr;

    if ( (ptr = malloc(size)) == NULL) {
        perror("[ERROR] Unix - malloc error.\n");
        exit(0);
    }
    return (ptr);
}

void Pipe(int *fds)
{
    if (pipe(fds) < 0) {
        perror("[ERROR] Unix - pipe error.\n");
        exit(0);
    }
}

pid_t Fork(void)
{
    pid_t   pid;
    if ( (pid = fork()) == -1) {
        perror("[ERROR] Unix - fork error");
        exit(0);
    }
    return (pid);
}

void Dup2(int fd1, int fd2)
{
    if (dup2(fd1, fd2) == -1) {
        perror("[ERROR] Unix - dup2 error");
        exit(0);
    }
}

/**
 * Append history to log.txt
 */

void Log(char *buf)
{
    FILE *fp = fopen("log.txt", "a");
    fprintf(fp, "%s", buf);
    fclose(fp);
}

void Chdir(const char *dir)
{
    if (chdir(dir) == -1) {
        perror("[ERROR] Unix - chdir error");
        exit(0);
    }
}

/**
 * Check error for setenv in stdlib, and then set override
 * to TRUE(1)
 */

void setenv2(const char *name, const char *value)
{
    if (setenv(name, value, TRUE) == -1) {
        perror("[ERROR] Unix - setenv error");
        exit(0);
    }
}

void Select(int nfds, fd_set *readfds, fd_set *writefds,
                          fd_set *exceptfds, struct timeval *timeout)
{
    int result;
    if ( (result = select(nfds, readfds, writefds,
                            exceptfds, timeout)) < 0 && (errno != EINTR)) {
        perror("[ERROR] Unix - select error");
    }
}

void Unlink(char const *pathname)
{
    if (unlink(pathname) == -1) {
        perror("[ERROR] Unix - close error.\n");
        exit(EXIT_FAILURE);
    }
}

void Execl(const char *path, char *arg)
{
    if (execl(path, arg, NULL) < 0) {
        perror("[ERROR] Unix - execl error.\n");
    }
}

/* =============================================== */
/* share memory */
/* =============================================== */

int Shmget(key_t key, size_t size, int flags)
{
    int     rc;
    if ((rc = shmget(key, size, flags)) == -1) {
        perror("[ERROR] UNIX - shmget error");
    }
    return rc;
}

void *Shmat(int id, const void * shmaddr, int flags)
{
    void *ptr;

    if ( (ptr = shmat(id, shmaddr, flags)) == (void *)(-1)) {
        perror("[ERROR] UNIX - shmat error");
    }
    return (ptr);
}

void Shmdt(const void *shmaddr)
{
    if (shmdt(shmaddr) == -1) {
        perror("[ERROR] UNIX - shmdt error");
    }
}

void Shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    if (shmctl(shmid, cmd, buf) == -1) {
        perror("[ERROR] UNIX - shmctl error");
    }
}

/* =============================================== */
/* fifo */
/* =============================================== */

void Mknod(const char *pathname, mode_t mode, dev_t dev)
{
    if (mknod(pathname, mode, dev) < 0) {
        perror("[ERROR] UNIX - mknod error");
    }
}
