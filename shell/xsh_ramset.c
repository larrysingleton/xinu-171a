
#include <xinu.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * xsh_ramset - define DIST, SMALLCOST, and LARGECOST
 *------------------------------------------------------------------------
 */
shellcmd xsh_ramset(int nargs, char *args[]) {

    /*-----------------------------------------------------------*/
    /* For argument '--help', emit help about the 'ramset' command. */
    /*-----------------------------------------------------------*/
    if (nargs ==2 && strncmp(args[1], "--help", 7) == 0) {
        printf("Use: %s DIST SMALLCOST LARGECOST\n\n", args[0]);
        printf("Description:\n");
        printf("\t Sets the simulated disk heads seek costs\n");
        printf("Options:\n");
        printf("\t--help\t display this help and exit\n");
        return OK;
    }

    /*--------------------------------------*/
    /* Check for valid number of arguments. */
    /*--------------------------------------*/
    if (nargs != 4) {
        fprintf(stderr, "%s: wrong number of arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return SYSERR;
    }

    int32 dist = atoi(args[1]);
    int32 smallCost = atoi(args[2]);
    int32 largeCost = atoi(args[3]);

    /* Validate arguments */
    if (dist < 0 || smallCost < 0 || largeCost < 0) {
        fprintf(stderr, "%s: input parameters must be greater than zero\n", args[0]);
        return SYSERR;
    }


    control(RAM0, RAM_CTL_SET, dist * 65536 + smallCost, largeCost);

    return OK;
}
