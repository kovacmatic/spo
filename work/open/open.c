#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "file"
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); } // macro

void main() 
{
    int fd;
    // char buf[BUFFSIZE];

    // open or create
    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC);
    printf("%d\n", fd);
    if (fd < 0) 
    {
        err("open")
    }

    // write
    char myText[] = "this is random text";
    if (write(fd, myText, strlen(myText)) < 0)
    {
        err("write")
    }

    printf("program successfully executed!\n");
    
    // close
    if (close(fd) < 0) 
    {
        err("close")
    }
}