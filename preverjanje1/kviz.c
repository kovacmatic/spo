#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char buff[] = "12345"; 
    int fd = open("dat.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700); 
    ssize_t n = write(fd, buff, 5); 
    off_t offset = lseek(fd, (off_t) 1, SEEK_END);
    return 0;
}
