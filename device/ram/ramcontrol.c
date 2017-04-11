/* ramcontrol.c - ramcontrol */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ramcontrol  -  Control ram disk
 *------------------------------------------------------------------------
 */
devcall	ramcontrol(
        struct dentry	*devptr,	/* Entry in device switch table	*/
        int32	 func,			/* Function to perform		*/
        int32	 arg1,			/* Argument 1 for request	*/
        int32	 arg2			/* Argument 2 for request	*/
)
{
    struct	ramcblk	*typtr;		/* Pointer to ram control block	*/

    /* Process the request */

    switch ( func )	{
        case RAM_CTL_SET:
            /* Extract and save DIST, SMALLCOST, and large costLARGECOST */
            arg1 = DIST * 65536 + SMALLCOST;

            if(arg2 > 0) {
                LARGECOST = arg2;
            } else {
                return (devcall) SYSERR;
            }

            LARGECOST = arg2


        case RAM_CTL_ZERO:


        case RAM_CTL_READ:


        default:
            return (devcall)SYSERR;
    }
}

