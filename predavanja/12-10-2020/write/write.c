#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define BUFSIZE 100

void main()
{
    int fd, fd2, nr, nw;
    char buf[BUFSIZE];

    fd = open("file1.txt", O_RDONLY);
    if (fd < 0) 
    {
        perror("open");
    }

    // O_CREATE - create file, if not exists
    fd2 = open("file2", O_WRONLY | O_CREAT);
    if (fd < 0) 
    {
        perror("open write");
    }

    nr = read(fd, buf, 30);
    printf("n: %d\n", nr);
    if (nr < 0) {
        perror("read");
    }

    nw = write(fd2, buf, nr);
    if (nw != nr)
    {
        perror("write2");
    }
    write(STDIN_FILENO, buf, nr);

    close(fd);
}