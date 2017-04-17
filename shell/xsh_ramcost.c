/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 */

#include <xinu.h>
#include <stdlib.h>
#include <ramdisk.h>

/*------------------------------------------------------------------------
 * xsh_ramcost - displays the total head movement cost
 *------------------------------------------------------------------------
 */
shellcmd xsh_ramcost(int nargs, char *args[]) {

    int status;
    int cost=999;

    /*-----------------------------------------------------------*/
    /* For argument '--help', emit help about the 'ramzero' command. */
    /*-----------------------------------------------------------*/
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
        printf("Use: %s \n\n", args[0]);
        printf("Description:\n");
        printf("\t display the total head movement cost\n");
        printf("Options:\n");
        printf("\t--help\t display this help and exit\n");
        return OK;
    }

    status = control(RAM0, RAM_CTL_READ, (int) &cost, 0);
    fprintf(stdout, "ramcost: %d\n", cost);

    return status;
}
