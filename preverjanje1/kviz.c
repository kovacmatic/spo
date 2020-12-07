#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("dat.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
off_t offset = lseek(fd, (off_t) 1, SEEK_END);
    return 0;
}
