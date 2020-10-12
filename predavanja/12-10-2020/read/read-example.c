#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define BUFSIZE 100

void main()
{
    int fd, n;
    char buf[BUFSIZE];

    fd = open("file1.txt", O_RDONLY);
    if (fd < 0) 
    {
        perror("opendd");
    }

    n = read(fd, buf, 10);
    printf("n: %d\n", n);

    close(fd);
}