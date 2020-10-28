/*
Napišite v C-ju poenostavljeno verzijo ukaza ls -la, ki izpiše imena vseh datotek v trenutnem
imeniku ("."), kakor tudi tip datoteke (običajna, imenik, vtičnica, bločna, znakovna, ...), dostopna
dovoljenja (mode) in število trdih povezav (st_nlink).

• Ne pozabite na S_ISUID, S_ISGID in S_ISVTX.
• Uporaba "exec ls –la" ni dovoljena.
• Primer izpisa:
-rwsr-Sr-x 1 lsl
-rw-r--r-- 1 lsl.c
drwxr-xr-t 2 .
drwxr-xr-x 6 ..
-rw------T 1 stevila
*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

DIR* openCurrentDIR();
void printData(DIR* dp);
struct stat getStat(struct dirent *dirp);
char getFileType(mode_t st_mode);

int main(int argc, char const *argv[])
{
    DIR *dp;

    dp = openCurrentDIR();
    printData(dp);
    closedir(dp);

    return 0;
}

DIR* openCurrentDIR() {
    DIR *dp;
    dp = opendir("./");
    if (dp == NULL) {
        perror("open");
        exit(1);
    }
    return dp;
}

void printData(DIR* dp) {
    struct dirent *dirp;
    struct stat stat;

    while ((dirp = readdir(dp)) != NULL) {
        stat = getStat(dirp);

        // write output
        printf("%c%c%c%c%c%c%c%c%c%c %lu %s\n",
            getFileType(stat.st_mode),               // file type char
            (stat.st_mode & S_IRUSR) ? 'r' : '-',    // user read
            (stat.st_mode & S_IWUSR) ? 'w' : '-',    // user write
            ((stat.st_mode & S_IXUSR) && (stat.st_mode & S_ISUID)) ? 's' 
            : (stat.st_mode & S_IXUSR) ? 'x' 
            : (stat.st_mode & S_ISUID) ? 'S' 
            : '-',                                   // user execute / set-user-ID
            (stat.st_mode & S_IRGRP) ? 'r' : '-',    // group read
            (stat.st_mode & S_IWGRP) ? 'w' : '-',    // group write
            ((stat.st_mode & S_IXGRP) && (stat.st_mode & S_ISGID)) ? 's'
            : (stat.st_mode & S_IXGRP) ? 'x' 
            : (stat.st_mode & S_ISGID) ? 'S' 
            : '-',                                   // group execute / set-group-ID
            (stat.st_mode & S_IROTH) ? 'r' : '-',    // other read
            (stat.st_mode & S_IWOTH) ? 'w' : '-',    // other write
            ((stat.st_mode & S_IXOTH) && (stat.st_mode & S_ISVTX)) ? 't'
            : (stat.st_mode & S_IXOTH) ? 'x' 
            : (stat.st_mode & S_ISVTX) ? 'T' 
            : '-',                                   // other execute / sticky bit
            stat.st_nlink,                           // number of links 
            dirp->d_name);                           // name 

    }
}

struct stat getStat(struct dirent *dirp) {
    struct stat stats;
    if (stat(dirp->d_name, &stats) < 0) {
        perror("stat");
        exit(1);
    }

    return stats;
}

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