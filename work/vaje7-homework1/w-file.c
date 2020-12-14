#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void Close(int fd) {
    if (close(fd) < 0) {
        perror("close");
        exit(1);
    }
}

ssize_t Write(int fd, char *msg, ssize_t len) {
    ssize_t res;
    if ((res = write(fd, msg, len)) != len) {
        perror("write");
        exit(1);
    }

    return res;
}

ssize_t Read(int fd, char *msg, ssize_t len) {
    ssize_t res;
    if ((res = read(fd, msg, len)) < 0) {
        perror("write");
        exit(1);
    }

    return res;
}
