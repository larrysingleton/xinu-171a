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
            /* Validate inputs */
            if(
                    (arg1 / 65536) <= 0 ||              // Verify DIST will be greater than 0
                    (arg1 - (arg1 / 65536)) <= 0 ||     // Verify SMALLCOST will be greater than 0
                    arg2 <= 0 ||                        // Verify LARGECOST will be greater than 0
                    (arg1 - (arg1 / 65536)) <= LARGECOST// Verify SMALLCOST will be greater than LARGECOST) {
                return (devcall) SYSERR;
            }

            /* Extract and save DIST, SMALLCOST, and LARGECOST */
            DIST = arg1 / 65536;
            SMALLCOST = arg1 - DIST;
            LARGECOST = arg2;

            return;

        case RAM_CTL_ZERO:
            /* Sets total head move cost to 0 */
            TOTALCOST = 0;
            return;

        case RAM_CTL_READ:
            /* Stores the simulated RAM Disk movement cost */
            if(N <= DIST) {
                &arg1 = DIST * SMALLCOST;
            } else {
                &arg1 = DIST * LARGECOST;
            }
            return;

        default:
            return (devcall)SYSERR;
    }
}

