#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>     
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "file.example"
#define OTHER_FILE_NAME "other-file.example"
#define READ_BUFFER 8192
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(0); }

int main(void)
{
    int fd, other_fd, n, sz;
    char buf[READ_BUFFER]; // read buffer
    char text[] = "this is first text";
    char otherText[] = "\nthis is second text";

    remove(FILE_NAME);
    remove(OTHER_FILE_NAME);    

    // open or create new
    if ((fd = open(FILE_NAME, O_RDWR | O_CREAT)) < 0)
    {
        err("open")
    }

    // write some text
    if (write(fd, text, strlen(text)) < 0) 
    {
        err("write")
    }

    // at the end add string "this is second text"
    if (lseek(fd, 0, SEEK_END) < 0) 
    {
        err("lseek")
    }
    if (write(fd, otherText, strlen(otherText)) < 0) 
    {
        err("write")
    }

    // read & write to another file
    // 1. open new
    if ((other_fd = open(OTHER_FILE_NAME, O_RDWR | O_CREAT)) < 0)
    {
        err("open other")
    }

    // read from original and write into other
    lseek(fd, 0, SEEK_SET); // set on start of the file
    while ((n = read(fd, buf, READ_BUFFER)) > 0) 
    {
        if (write(other_fd, buf, n) != n) 
        {
            err("write other")
        }
    }

    // close
    close(fd);
    close(other_fd);
}