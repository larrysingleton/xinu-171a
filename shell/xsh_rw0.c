/* xsh_rw0.c - xsh_rw0 */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 512

/*------------------------------------------------------------------------
 * xsh_rw0 - write junk to a block from RAM0
 *------------------------------------------------------------------------
 */
shellcmd xsh_rw0(int nargs, char *args[]) {
    char buffer[RM_BLKSIZ];    /* buffer for a block */
    int status, k;

    /*-----------------------------------------------------------*/
    /* For argument '--help', emit help about the 'rw0' command. */
    /*-----------------------------------------------------------*/
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
        printf("Use: %s k\n\n", args[0]);
        printf("Description:\n");
        printf("\tWrite junk to block k on RAM0.\n");
        printf("Options:\n");
        printf("\t--help\t display this help and exit\n");
        return 0;
    }

    /*--------------------------------------*/
    /* Check for valid number of arguments. */
    /*--------------------------------------*/
    if (nargs != 2) {
        fprintf(stderr, "%s: wrong number of arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return 1;
    }

    k = atoi(args[1]);
    if (k < 0 || k >= RM_BLKS) {
        fprintf(stderr, "%s: bad block number (%s); must be in 0..%d\n", args[1], RM_BLKS - 1);
        return 1;
    }

    memset(buffer, 'x', RM_BLKSIZ);        /* fill buffer with 'x' */
    status = ramwrite(RAM0, buffer, k);

    printf("ramwrite returned %d\n", status);

    return 0;
}
