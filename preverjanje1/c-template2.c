// #include <sys/socket.h> // delo s socketi
// #include <netinet/in.h> // sockaddr_in za bind pri socketih
// #include <arpa/inet.h> // inet_ntop


///  SOCKETS

/* ********** SNIPPET (server): pridobivanje IP naslova clienta na serverju (inet_ntop) */
/*
___________________________________________________________________________________
#include <arpa/inet.h>
#include <netinet/in.h>

char str[INET_ADDRSTRLEN];
struct sockaddr_in clientaddr;

printf("Client connection accepted. IP: %s, port:%d\n", 
        inet_ntop(AF_INET, &clientaddr.sin_addr, str, INET_ADDRSTRLEN), 
        ntohs(clientaddr.sin_port));
___________________________________________________________________________________
*/