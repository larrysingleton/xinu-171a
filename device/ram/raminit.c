/* raminit.c  -  raminit */

/*----------------------------------------------------------------------*/
/* This version of raminit contains suggestions about the modifications */
/* that might be included to add the disk caching needed for the fourth */
/* assignment for CSCI 8530, Spring 2017. Each suggested modification   */
/* is marked with a comment including "XXX".                            */
/*----------------------------------------------------------------------*/

#include <xinu.h>
#include <ramdisk.h>

extern uint32 ram_pos;		/* block where simulated head is positioned. */
extern uint32 ram_dist;		/* dist above which ram_largecost applies. */
extern uint32 ram_smallcost;	/* cost to move 1..ram_dist "blocks" */
extern uint32 ram_largecost;	/* cost to move more than ram_dist "blockst" */
extern uint32 ram_seek_cost;	/* accumulated "head movement" cost */

struct	ramdisk	Ram;

/*------------------------------------------------------------------------
 *  raminit  -  Initialize the remote disk system device
 *------------------------------------------------------------------------
 * Note that it is important that any RAM disk device be initialized
 * before any other devices that use it (like the local filesystem).
 * This is because raminit fills the RAM disk with "hopeless" just, and
 * any other initialization will be trashed.
 *
 * Basically this means that the device number for a RAM disk must be
 * smaller than that of any device that initializes the RAM disk content.
 */
devcall	raminit (
    struct dentry*devptr		/* Entry in device switch table	*/
)
{
    int index;

    /*-----------------------------------*/
    /* Fill the ram disk with something. */
    /*-----------------------------------*/
    memcpy(Ram.disk, "hopeless", 8);
    memcpy( &Ram.disk[8], Ram.disk, RM_BLKSIZ * RM_BLKS - 8);

    /*--------------------------*/
    /* Initialize the Ram Cache */
    /*--------------------------*/
    RamCacheHead = -1;
    RamCacheFree = 0;
    for (index=0; index<RM_CACHE; index++) {
        RamCache[index].next = index + 1;
        RamCache[index].blockno = -1;
        memcpy(RamCache[index].block, 0, RM_BLKSIZ);
    }
    RamCache[RM_CACHE].next = -1;

    /*------------------------------------------------------*/
    /* Initialize values for the disk seek cost simulation. */
    /*------------------------------------------------------*/
    ram_pos = 0;
    ram_dist = 0;
    ram_smallcost = 0;
    ram_largecost = 1;
    ram_seek_cost = 0;

    /*----------------------------------------------------*/
    /* XXX Build a free list of the blocks for the cache. */
    /*----------------------------------------------------*/

    return OK;
}
