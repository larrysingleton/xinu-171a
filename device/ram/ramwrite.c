/* ramwrite.c  -  ramwrite */

/*-----------------------------------------------------------------------*/
/* This version of ramwrite contains suggestions about the modifications */
/* that might be included to add the disk caching needed for the fourth  */
/* assignment for CSCI 8530, Spring 2017. Each suggested modification    */
/* is marked with a comment including "XXX".                             */
/*-----------------------------------------------------------------------*/

#include <xinu.h>
#include <ramdisk.h>
#include <stdlib.h>

extern uint32 ram_pos;        /* block to which simulated head is positioned. */
extern uint32 ram_dist;       /* distance above which ram_largecost applies. */
extern uint32 ram_smallcost;  /* cost to move 1..ram_dist "blocks" */
extern uint32 ram_largecost;  /* cost to move more than ram_dist "blockst" */
extern uint32 ram_seek_cost;    /* accumulated "head movement" cost */

/*------------------------------------------------------------------------
 * ramwrite  -  Write a block to a ram disk
 *------------------------------------------------------------------------
 */
devcall	ramwrite (
    struct dentry *devptr,	/* Entry in device switch table	*/
    char *buff,			/* Buffer containing a block	*/
    int32 blk			/* Block number to write	*/
)
{
    int32 bpos;			/* Byte position of blk		*/
    int32 dist;                 /* distance heads moved         */

    /*---------------------------------------------*/
    /* Perform simulated "seek" to right position. */
    /*---------------------------------------------*/
    dist = abs(blk - ram_pos);
    if (dist > 0) { 	        /* did the "head" move? */
        if (dist <= ram_dist)
            ram_seek_cost += dist * ram_smallcost;
        else
            ram_seek_cost += dist * ram_largecost;
    }

    /*--------------------------------------------------*/
    /* Perform the data transfer from buffer to "disk". */
    /*--------------------------------------------------*/
    bpos = RM_BLKSIZ * blk;
    memcpy(&Ram.disk[bpos], buff, RM_BLKSIZ);

    /*------------------------------------------*/
    /* Update the simulated disk head location. */
    /*------------------------------------------*/
    ram_pos = blk + 1;          /* ready to access next block with no seek */
    if (ram_pos >= RM_BLKS)     /* wraparound to beginning of disk */ 
        ram_pos = 0;

    /*----------------------------------------------------------------*/
    /* XXX If the block is in the cache, update its content and move  */
    /* XXX it to the head of the list of cached entries, if it is not */
    /* XXX already there.                                             */
    /*----------------------------------------------------------------*/

    return OK;
}
