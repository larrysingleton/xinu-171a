/* ramcontrol.c - ramcontrol */

/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 */

#include <xinu.h>
#include <ramdisk.h>

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
    int32 dist=0;
    int32 smallCost=0;

    /* Process the request */
    switch ( func )	{

        case RAM_CTL_SET: 
            /* Extract and save DIST, SMALLCOST, and LARGECOST */
            dist = arg1 >> 16;
            smallCost = arg1 & 0xffff;

            /* Validate inputs */
            if(dist <= 0 || smallCost <= 0 || arg2 <= 0 || smallCost <= arg2) {
                return (devcall) SYSERR;
            }

            DIST = dist;
            SMALLCOST = smallCost;
            LARGECOST = arg2;

            return (devcall) OK;

        case RAM_CTL_ZERO: 
            /* Sets total head move cost to 0 */
            TOTALCOST = 0;
            return (devcall) OK;

        case RAM_CTL_READ: 
            /* Sets the simulated RAM Disk movement cost */
            /* saved in arg1 as a pointer to an integer */
            *((int *)arg1) = TOTALCOST;
            return (devcall) OK;

        default:
            return (devcall)SYSERR;
    }
}

