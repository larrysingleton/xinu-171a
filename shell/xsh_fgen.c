/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 */

#include <xinu.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * xsh_fgen - file generation
 *------------------------------------------------------------------------
 */
shellcmd xsh_fgen(int nargs, char *args[]) {

    int fd;
    char *fileName;
    int fileSize;
    int status;

    /*-----------------------------------------------------------*/
    /* For argument '--help', emit help about the 'fgen' command. */
    /*-----------------------------------------------------------*/
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
        printf("Use: %s fileName fileSize\n\n", args[0]);
        printf("Description:\n");
        printf("\t generate a file named fileName with size fileSize\n");
        printf("Options:\n");
        printf("\t--help\t display this help and exit\n");
        return OK;
    }

    /*--------------------------------------*/
    /* Check for valid number of arguments. */
    /*--------------------------------------*/
    if (nargs != 3) {
        fprintf(stderr, "%s: wrong number of arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return SYSERR;
    }

    fileName = args[1];
    fileSize = atoi(args[2]);

    /* Validate arguments */
    if (fileSize < 0 ) {
        fprintf(stderr, "%s: fileSize must be a positive integer\n", args[0]);
        return SYSERR;
    }

    fd = open(NAMESPACE, fileName, "w");
    if (fd == SYSERR) {
        fprintf(stderr, "%s: unable to open file: %s\n", args[0], fileName);
        return SYSERR;
    } 

    /* truncate the file to zero size */
    control(fd, F_CTL_TRUNC, 0, 0);

    while (fileSize > 0) {
        status = write(fd, "X", 1);
        if (status == SYSERR) {
            fprintf(stderr, "%s: unable to write to file[%s] of size[%d]", args[0], fileName, fileSize);
        }
        --fileSize;
    }

    if (close(fd) == SYSERR) {
        fprintf(stderr, "%s: unable to close file: %s", args[0], fileName);
        return SYSERR;
    }

    return OK;
}
