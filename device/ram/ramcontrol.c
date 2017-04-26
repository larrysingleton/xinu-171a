/* ramcontrol.c - ramcontrol */

/*-------------------------------------------------------------------------*/
/* This needs no changes for the caching version of the RAM device driver. */
/*-------------------------------------------------------------------------*/

#include <xinu.h>
#include <ramdisk.h>

uint32 ram_pos;		/* block to which simulated head is positioned. */
uint32 ram_dist;	/* distance above which ram_largecost applies. */
uint32 ram_smallcost;	/* cost to move 1..ram_dist "blocks" */
uint32 ram_largecost;	/* cost to move more than ram_dist "blockst" */
uint32 ram_seek_cost;	/* accumulated "head movement" cost */

/*------------------------------------------------------------------------
 *  ramcontrol  -  Control a ram device
 *------------------------------------------------------------------------
 */
devcall	ramcontrol(
    struct dentry*devptr,	/* Entry in device switch table	*/
    int32 func,			/* Function to perform		*/
    int32 arg1,			/* Argument 1 for request	*/
    int32 arg2			/* Argument 2 for request	*/
)
{
    /*----------------------*/
    /* Process the request. */
    /*----------------------*/

    switch (func) {
	case RAM_CTL_SET:
	    ram_dist = arg1 / 65536;
	    ram_smallcost = arg1 & 0xffff;
	    ram_largecost = arg2;
	    return (devcall)OK;

	case RAM_CTL_ZERO:
	    ram_seek_cost = 0;
	    return (devcall)OK;

	case RAM_CTL_READ:
	    *(uint32 *)arg1 = ram_seek_cost;
	    return (devcall)OK;

	default:
	    return (devcall)SYSERR;
    }
}
