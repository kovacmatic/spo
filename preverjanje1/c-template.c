#include <stdio.h>
#include <stdlib.h>
// #include <errno.h> // errno - za preverjanje readdir
// #include <sys/stat.h> // stat; chmod
// #include <dirent.h> // DIR
// #include <fcntl.h> // open
// #include <unistd.h> // read; lseek
// #include <stdbool.h> // bool
// #include <string.h> // strlen; strcmp (primerjava stringov)
// #include <ctype.h> // isdigit
// #include <dirent.h> // DIR

// #include <sys/types.h>
// #include <sys/wait.h> // waitpit, wait

// dobi string length - strlen 
// primerjava stringov - strcmp(inputPermissions, "u+s") == 0 ;  0 - success

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
// #define VAR VALUE

int main(int argc, char const *argv[])
{
    return 0;
}


/* Usefull functions

// nastavi pravice
void setPermissions(const char *inputDIRName, long permissions) {
    if (chmod(inputDIRName, permissions) < 0) {
        perror("chmod");
        exit(1);
    }
}

// validate aguments count
void validateArgsCount(int argc) {
    if (argc != ARGS_COUNTR_REQUIRED) {
        error("Invalid argument count.")
    }
}

// Odpri poljuben file z določenimi pravicami
int openFile(const char *fileName, int flags) {
    int fd;
    if ((fd = open(fileName, flags, 0777)) < 0) { // all premissions - 0777
        perror("open");
        exit(1);
    }

    return fd;
}

// Izpiši STDIN to STDOUT
void printStdinToStdout() {
    int readSize;
    char buff[BUFFER];
    do {
        readSize = read(STDIN_FILENO, buff, BUFFER);
        if (readSize < 0) {
            perror("read");
            exit(1);
        }

        if (write(STDOUT_FILENO, buff, readSize) != readSize) {
            perror("write STDIN -> STDOUT");
            exit(1);
        }
        
    } while (readSize > 0);
}

// Odpri trenutni direktorij
DIR* openCurrentDIR() {
    DIR *dp;
    dp = opendir("./");
    if (dp == NULL) {
        perror("open");
        exit(1);
    }
    return dp;
}

// Pridobi directory stat podatke
struct stat getStat(struct dirent *dirp) {
    struct stat stats;
    if (stat(dirp->d_name, &stats) < 0) {
        perror("stat");
        exit(1);
    }

    return stats;
}

// indidates if input is number
bool isInputDigit(const char *text) {
    for (int i = 0; i < strlen(text); i++) {
        if (isdigit(text[i]) == 0) {
            return false;
        }
    }

    return true;
}

// vrne za kakšen file gre - datoteka, fifo, socket, ipd... 
char getFileType(mode_t st_mode) {
    if (S_ISREG(st_mode)) {
        return '-';
    } else if (S_ISDIR(st_mode)) {
        return 'd';
    } else if (S_ISCHR(st_mode)) {
        return 'c';
    } else if (S_ISBLK(st_mode)) {
        return 'b';
    } else if (S_ISFIFO(st_mode)) {
        return 'p';
    } else if (S_ISLNK(st_mode)) {
        return 'l';
    } else if (S_ISSOCK(st_mode)) {
        return 's';
    } else {
        fprintf(stderr, "Unhandled st_mode type");
        exit(1);
    }
}

// izpiši dovoljenja
void printPermissions(const char *fileName) {
    struct stat stat;
    stat = getStat(fileName);

     // write output
    printf("Nova dovoljenja: %c%c%c%c%c%c%c%c%c\n",
        (stat.st_mode & S_IRUSR) ? 'r' : '-',   // user read
        (stat.st_mode & S_IWUSR) ? 'w' : '-',   // user write
        ((stat.st_mode & S_IXUSR) && (stat.st_mode & S_ISUID)) ? 's' 
        : (stat.st_mode & S_IXUSR) ? 'x'        // user execute 
        : (stat.st_mode & S_ISUID) ? 'S'        // set-user-ID 
        : '-',                                  
        (stat.st_mode & S_IRGRP) ? 'r' : '-',   // group read
        (stat.st_mode & S_IWGRP) ? 'w' : '-',   // group write
        ((stat.st_mode & S_IXGRP) && (stat.st_mode & S_ISGID)) ? 's'
        : (stat.st_mode & S_IXGRP) ? 'x'        // group execute
        : (stat.st_mode & S_ISGID) ? 'S'        // set-group-ID
        : '-',                                  
        (stat.st_mode & S_IROTH) ? 'r' : '-',   // other read
        (stat.st_mode & S_IWOTH) ? 'w' : '-',   // other write
        ((stat.st_mode & S_IXOTH) && (stat.st_mode & S_ISVTX)) ? 't'
        : (stat.st_mode & S_IXOTH) ? 'x'        // other execute 
        : (stat.st_mode & S_ISVTX) ? 'T'        // sticky bit
        : '-');                                                            
}

 */