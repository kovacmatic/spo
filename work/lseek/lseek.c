#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define err_sys(mess) { fprintf(stderr, "Error: %s.", mess); exit(1); }

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    if ((fd = creat("file.hole", 0666)) < 0)
        err_sys("creat error") 
    
    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error") 
        
    if (lseek(fd, 40, SEEK_SET) == -1)
        err_sys("lseek error") 
    
    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error") return 0;
}
