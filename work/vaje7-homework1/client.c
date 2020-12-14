#include "w-socket.c"
#include "w-file.c"
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1948
#define BUFFER 100

void processMessages(int sockFd);

int main(int argc, char const *argv[]) {
    int sockFd;
    struct sockaddr_in servaddr;

    // create socket
    sockFd = Socket(AF_INET, SOCK_STREAM, 0);

    // connect
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);    
    Connect(sockFd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("Connected!\n");

    processMessages(sockFd);
    return 0;
}

void processMessages(int sockFd) {
    int sendSize, receiveSize;
    char sendLine[BUFFER], receiveLine[BUFFER];

    do {
        // read stdin
        sendSize = Read(STDIN_FILENO, sendLine, BUFFER);

        // write to socket fd
        Write(sockFd, sendLine, sendSize);

        // read respone
        receiveSize = Read(sockFd, receiveLine, BUFFER);
        Write(STDOUT_FILENO, receiveLine, receiveSize); 
        printf("\n");  
    } while (sendSize > 0);
}