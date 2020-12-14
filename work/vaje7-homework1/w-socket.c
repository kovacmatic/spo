#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <netinet/in.h>

int GetSockName(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
    int n;
    if ((n = getsockname(sockfd, addr, &addrlen)) < 0) {
        perror("getsockname");
        exit(1);
    }

    return n;
}

int Socket(int family, int type, int protocol) {
    int n;
    if ((n = socket(family, type, protocol)) < 0) {
        perror("socket error");
        exit(1);
    }

    return n;
}

int Connect(int sockFd, struct sockaddr *addr, socklen_t addrlen) {
    int n;
    if ((n = connect(sockFd, addr, addrlen)) < 0) {
        perror("connect");
        exit(1);
    }

    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        perror("listen");
        exit(1);
    }
}

void Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        perror("listen");
        exit(1);
    }
}

int Accept(int fd, struct sockaddr *sa, socklen_t salenptr) {
    int n;
    again:
    if ((n = accept(fd, sa, &salenptr)) < 0) {
        if (errno == ECONNABORTED) {
            goto again;
        } else {
            perror("accept");
            exit(1);
        }
    }
    return (n);
}