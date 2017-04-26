/* ramread.c  -  ramread */

/*----------------------------------------------------------------------*/
/* This version of ramread contains suggestions about the modifications */
/* that might be included to add the disk caching needed for the fourth */
/* assignment for CSCI 8530, Spring 2017. Each suggested modification   */
/* is marked with a comment including "XXX".                            */
/*----------------------------------------------------------------------*/

#include <xinu.h>
#include <ramdisk.h>
#include <stdlib.h>

extern uint32 ram_pos;        /* block to which simulated head is positioned. */
extern uint32 ram_dist;       /* distance above which ram_largecost applies. */
extern uint32 ram_smallcost;  /* cost to move 1..ram_dist "blocks" */
extern uint32 ram_largecost;  /* cost to move more than ram_dist "blocks" */
extern uint32 ram_seek_cost;  /* accumulated "head movement" cost */

/*------------------------------------------------------------------------
 * ramread  -  Read a block from a ram disk
 *------------------------------------------------------------------------
 */
devcall	ramread (
    struct dentry *devptr,	/* Entry in device switch table	*/
    char *buff,			/* Buffer to hold disk block	*/
    int32 blk			/* Block number of block to read*/
)
{
    int32 bpos;			/* Byte position of blk		*/
    int32 dist;			/* distance heads moved		*/

    /*------------------------------------------------------------*/
    /* XXX Determine if the block is in the cache. If it is, copy */
    /* XXX its content to the user buffer (buff). If the entry is */
    /* XXX not at the head of the list of cached entries, then    */
    /* XXX move it there. Then return.                            */
    /*------------------------------------------------------------*/

    /*-----------------------------------------------------------------*/
    /* Block not in cache; perform simulated "seek" to right position. */
    /*-----------------------------------------------------------------*/
    dist = abs(blk - ram_pos);
    if (dist > 0) {		/* did the "head" move? */
	if (dist <= ram_dist)
	    ram_seek_cost += dist * ram_smallcost;
	else
	    ram_seek_cost += dist * ram_largecost;
    }

    /*--------------------------------------------------*/
    /* Perform the data transfer from "disk" to buffer. */
    /*--------------------------------------------------*/
    bpos = RM_BLKSIZ * blk;
    memcpy(buff, &Ram.disk[bpos], RM_BLKSIZ);

    /*------------------------------------------*/
    /* Update the simulated disk head location. */
    /*------------------------------------------*/
    ram_pos = blk + 1;		/* ready to access next block with no seek */
    if (ram_pos >= RM_BLKS)	/* wraparound to beginning of disk */
	ram_pos = 0;

    /*--------------------------------------------------------------*/
    /* XXX Put the block in the cache. If there is an unused block, */
    /* XXX get one of them to use. Otherwise use the last block in  */
    /* XXX the list of cached entries (that is, the least recently  */
    /* XXX used entry). Copy the data just read from the "disk" to  */
    /* XXX to the entry, and make the entry the head of the list.   */
    /*--------------------------------------------------------------*/

    return OK;
}
