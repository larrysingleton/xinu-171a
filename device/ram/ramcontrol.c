/* ramcontrol.c - ramcontrol */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ramcontrol  -  Control ram disk
 *------------------------------------------------------------------------
 */

extern int32 DIST;
extern int32 SMALLCOST;
extern int32 LARGECOST;
extern int32 TOTALCOST;

devcall	ramcontrol(
        struct dentry	*devptr,	/* Entry in device switch table	*/
        int32	 func,			/* Function to perform		*/
        int32	 arg1,			/* Argument 1 for request	*/
        int32	 arg2			/* Argument 2 for request	*/
) {
    /* Process the request */
    switch ( func )	{
        case RAM_CTL_SET:
            /* Extract and save DIST, SMALLCOST, and LARGECOST */
            printf("RAM_CTL_SET called %d %d", arg1, arg2);

            int32 dist = arg1 >> 16;
            int32 scost = arg1 & 0xffff;

            /* Validate inputs */
            if(dist <= 0 || scost <= 0 || arg2 <= 0 || scost <= arg2) {
                return (devcall) SYSERR;
            }

            DIST = dist;
            SMALLCOST = scost;
            LARGECOST = arg2;

            return (devcall) OK;

        case RAM_CTL_ZERO:
            /* Sets total head move cost to 0 */
            TOTALCOST = 0;
            return (devcall) OK;

        case RAM_CTL_READ:
            printf("RAM_CTL_READ called %d %d", arg1, arg2);
            /* Stores the simulated RAM Disk movement cost */
            if(1 <= DIST) { // Need to figure out where to get 1 from, supposed to be N.
                arg1 = DIST * SMALLCOST;
            } else {
                arg1 = DIST * LARGECOST;
            }
            printf("RAM_CTL_READ after %d %d", arg1, arg2);
            return (devcall) OK;

        default:
            return (devcall)SYSERR;
    }
}

