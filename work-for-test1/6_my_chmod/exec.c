/*
• Napišite program mojChmod, ki bo deloval podobno kot terminalski ukaz chmod.
• Ob zagonu programa podamo dva argumenta:
    ▪ nova dovoljenja (mode) in
    ▪ ime datoteke, katere dovoljenja spreminjamo.
• Dovoljenja podamo kot osmiško število (npr. 0777, 0644, 4700) ali pa kot
tričrkovni niz. Dovolimo zgolj štiri oblike nizov:
    ▪ u+s (postavi bit "Set-User-ID")
    ▪ u-s (pobriši bit "Set-User-ID")
    ▪ g+s (postavi bit "Set-Group-ID")
    ▪ g-s (pobriši bit "Set-Group-ID")
• Program naj spremeni dovoljenja podane datoteke in ob izhodu izpiše
nova dovoljenja.
• Primer delovanja programa:
    $ ll test
    -rwxr----- 1 student student 0 2012-11-05 10:47 test*
    $ ./mojChmod 0664 test
    Nova dovoljenja: rw-rw-r--
    $ ./mojChmod g+s test
    Nova dovoljenja: rw-rwSr--
    $ ./mojChmod 4700 test
    Nova dovoljenja: rws------
    $ ./mojChmod u-s test
    Nova dovoljenja: rwx------
*/

// CALL EXAMPLE:
// cd ~
// /mnt/c/data/d/faks/spo/work-for-test1/6_my_chmod/exec u-s /mnt/c/data/d/faks/spo/work-for-test1/6_my_chmod/test.txt

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgsCount(int argc);
bool isInputDigit(const char *text);
void setDigitPermissions(const char *permissionsStr, const char *fileName);
void setStringPermissions(const char *permissionsStr, const char *fileName);
void setPermissions(long permissions, const char *fileName);
void printPermissions(const char *fileName);
struct stat getStat(const char *fileName);

int main(int argc, char const *argv[])
{
    const char *fileName, *permissionsStr;
    
    validateArgsCount(argc);

    permissionsStr = argv[1];
    fileName = argv[2];

    if (isInputDigit(permissionsStr)) {
        setDigitPermissions(permissionsStr, fileName);
    } else {
        setStringPermissions(permissionsStr, fileName);
    }

    printPermissions(fileName);

    return 0;
}

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

struct stat getStat(const char *fileName) {
    struct stat stats;
    if (stat(fileName, &stats) < 0) {
        perror("stat");
        exit(1);
    }

    return stats;
}

void setDigitPermissions(const char *permissionsStr, const char *fileName) {

    // cast to digit
    long permissions;
    permissions = strtol(permissionsStr, NULL, 8);

    // set
    setPermissions(permissions, fileName);
}

void setStringPermissions(const char *permissionsStr, const char *fileName) {
    struct stat stat;
    stat = getStat(fileName);
    if (strcmp(permissionsStr, "u+s") == 0) {
        setPermissions((stat.st_mode | S_ISUID), fileName);

    } else if (strcmp(permissionsStr, "u-s") == 0) {
        setPermissions((stat.st_mode & ~S_ISUID), fileName);
    } else if (strcmp(permissionsStr, "g+s") == 0) {
        setPermissions((stat.st_mode | S_ISGID), fileName);
    } else if (strcmp(permissionsStr, "g-s") == 0) {
        setPermissions((stat.st_mode & ~S_ISGID), fileName);
    }
}

void setPermissions(long permissions, const char *fileName) {
    if (chmod(fileName, permissions) < 0) {
        perror("chmod");
        exit(1);
    }
}
void validateArgsCount(int argc) {
    if (argc != 3) {
        error("Invalid argument count. Example: ./exec [permissions] [file name]")
    }
}

bool isInputDigit(const char *text) {
    for (int i = 0; i < strlen(text); i++) {
        if (isdigit(text[i]) == 0) {
            return false;
        }
    }

    return true;
}
