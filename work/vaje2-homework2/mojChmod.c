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
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgs(int argc);
DIR* getDIR(const char *input);
bool isInputDigit(const char *text);
void modifyPermissions(const char *inputDIRName, const char *inputPermissions);
void printResults(const char *inputDIRName);
struct stat getStat(const char *inputDIRName);
void setPermissions(const char *inputDIRName, long permissions);

int main(int argc, char const *argv[])
{
    const char *inputDIRName, *inputPermissions;
    inputDIRName = argv[2];
    inputPermissions = argv[1];

    validateArgs(argc);    
    modifyPermissions(inputDIRName, inputPermissions);
    printResults(inputDIRName);

    return 0;
}

void validateArgs(int argc) {
    if (argc != 3) {
        error("Invalid aruments count. Two arguments are excepted.")
    }
}

void modifyPermissions(const char *inputDIRName, const char *inputPermissions) {
    long permissions;
    struct stat stat;
    stat = getStat(inputDIRName); 

    if (isInputDigit(inputPermissions)) { // check if permissions is number only
        permissions = strtol(inputPermissions, NULL, 8);
        printf("%d\n", (int)permissions);
        setPermissions(inputDIRName, permissions);
    } else {
        if (strcmp(inputPermissions, "u+s") == 0) { // set Set-User-ID
            setPermissions(inputDIRName, (stat.st_mode | S_ISUID));
        } else if (strcmp(inputPermissions, "u-s") == 0) { // remove Set-User-ID
            setPermissions(inputDIRName, (stat.st_mode & ~S_ISUID));
        } else if (strcmp(inputPermissions, "g+s") == 0) { // set Set-Group-ID
            setPermissions(inputDIRName, (stat.st_mode | S_ISGID));
        } else if (strcmp(inputPermissions, "g-s") == 0) { // remove Set-Group-ID
            setPermissions(inputDIRName, (stat.st_mode & ~S_ISGID));
        } else {
            error("Invalid permission arguments. Only u+s, u-s, g+s and g-s are allowed.");
        }
    }
}

void setPermissions(const char *inputDIRName, long permissions) {
    if (chmod(inputDIRName, permissions) < 0) {
        perror("chmod");
        exit(1);
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

void printResults(const char *inputDIRName) {    
    struct stat stat;
    stat = getStat(inputDIRName); 
    printf("Nova dovoljenja: %c%c%c%c%c%c%c%c%c\n",
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
            : '-');
}

struct stat getStat(const char *inputDIRName) {
    struct stat stats;
    if (stat(inputDIRName, &stats) < 0) {
        perror("stat");
        exit(1);
    }

    return stats;
}
