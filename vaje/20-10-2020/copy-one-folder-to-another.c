#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFSIZE 512 // velikost bufferja

#define err(arg) { fprintf(stderr, "%s\n", arg); exit(1); }
#define sys_err(arg) { perror(arg); exit(1); }
 
int main(int argc, char const *argv[])
{
    int fd;
    // char bigbuff[BUFFSIZE];

    // preveri število argumentov
    if (argc != 3) {
        err("Count of input arguments is invalid.");
    }

    // poiskusi odprti datoteko
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        sys_err(argv[1]);
    }



    return 0;
}
