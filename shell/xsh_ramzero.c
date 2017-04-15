/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 */

#include <xinu.h>
#include <stdlib.h>
#include <ramdisk.h>

/*------------------------------------------------------------------------
 * xsh_ramzero - set the total head movement cost for the RAM disk to zero
 *------------------------------------------------------------------------
 */
shellcmd xsh_ramzero(int nargs, char *args[]) {

    int status;

    /*-----------------------------------------------------------*/
    /* For argument '--help', emit help about the 'ramzero' command. */
    /*-----------------------------------------------------------*/
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
        printf("Use: %s \n\n", args[0]);
        printf("Description:\n");
        printf("\t Set the total head movement cost for the RAM disk to zero\n");
        printf("Options:\n");
        printf("\t--help\t display this help and exit\n");
        return OK;
    }

    status = control(RAM0, RAM_CTL_ZERO, 0, 0);

    return status;
}
