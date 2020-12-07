#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
#define ARGS_COUNT_REQUIRED 2
#define BUFSIZE 1

void validateArgsCount(int argc);
int openFile(const char *fileName, int flags);
off_t lseekFile(int fd, int offset, int whence);
void modifyPermissions(const char *fileName);
void setPermissions(const char *fileName, long permissions);
struct stat getStat(const char *fileName);
void printWordCount(const char *fileName);

int main(int argc, char const *argv[])
{
    int fd, currentOffset, endOffset, position = 0, posToSkip = -1, spaceInserted = 0;
    char buf[BUFSIZE];

    validateArgsCount(argc);
    fd = openFile(argv[1], O_RDWR);

    // set permissions
    modifyPermissions(argv[1]);

    // gets the end offset number
    endOffset = lseekFile(fd, 0, SEEK_END);

    do {
        // move to start
        currentOffset = lseekFile(fd, position++, SEEK_SET);

        // read one char
        if (read(fd, buf, 1) == -1) {
            perror("read");
            exit(1);
        }

        if (posToSkip < 0) {

            // read nr of spaces
            posToSkip = atoi(buf);
            spaceInserted = 0;

            // set space
            lseekFile(fd, -1, SEEK_CUR);
            if (write(fd, " ", 1) != 1) {
                perror("write space");
                exit(1);
            }
            lseekFile(fd, 1, SEEK_CUR);
        } else {
            if (spaceInserted < posToSkip && posToSkip != -1) {
                ++spaceInserted;
            } 
            
            if (spaceInserted == posToSkip) {
                // reset position
                posToSkip = -1;
                spaceInserted = 0;
            }
        }
    }
    while (currentOffset < endOffset - 1); // read until start offset

    printWordCount(argv[1]);

    close(fd);
    return 0;
}

off_t lseekFile(int fd, int offset, int whence) {
    off_t resOffset;
    if ((resOffset = lseek(fd, offset, whence)) == -1) {
        perror("lseek");
        exit(1);
    }

    return resOffset;
}

void validateArgsCount(int argc) {
    if (argc != ARGS_COUNT_REQUIRED) {
        error("Invalid arguments count.")
    }
}

int openFile(const char *fileName, int flags) {
    int fd;
    if ((fd = open(fileName, flags, 0777)) < 0) { // all premissions - 0777
        perror("open");
        exit(1);
    }

    return fd;
}

void modifyPermissions(const char *fileName) {    
    struct stat stat;
    stat = getStat(fileName);

    setPermissions(fileName, (stat.st_mode & ~S_IWGRP)); // group write
    setPermissions(fileName, (stat.st_mode & ~S_IWOTH)); // other write
}

void setPermissions(const char *fileName, long permissions) {
    if (chmod(fileName, permissions) < 0) {
        perror("chmod");
        exit(1);
    }
}

struct stat getStat(const char *fileName) {
    struct stat stats;
    if (stat(fileName, &stats) < 0) {
        perror("stat");
        exit(1);
    }

    return stats;
}

void printWordCount(const char *fileName) {
    pid_t pid;
    int fd[2];
    char buff[100];
    
    // make pipe
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    // create child
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) { // executing child process logic
        close(fd[0]);

        // STDOUT to write (fd[1])
        if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
            perror("dup2 (STDOUT -> write)");
            exit(1);
        }

        execlp("wc", "-w", fileName, NULL);
        perror("wc");
        exit(127);
    } else {
        close(fd[1]);
        if (waitpid(pid, NULL, 0) < 0) { // wait kiddo
            perror("waitpid");
            exit(1);
        }

        printf("Stevilo besed v datoteki ");
        while(read(fd[0], buff, 1000) > 0) {
            printf("%s\n", buff);
        }

        // running out of time :(
    }

}