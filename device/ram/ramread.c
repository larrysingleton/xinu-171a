/* ramread.c  -  ramread */

/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
 */

#include <xinu.h>
#include <ramdisk.h>

extern int POSITION;
extern int DIST;
extern int TOTALCOST;
extern int SMALLCOST;
extern int LARGECOST;

/*------------------------------------------------------------------------
 * ramread  -  Read a block from a ram disk
 *------------------------------------------------------------------------
 */
devcall	ramread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer to hold disk block	*/
	  int32	blk			/* Block number of block to read*/
	)
{
	int32	bpos;			/* Byte position of blk		*/

	bpos = RM_BLKSIZ * blk;
	memcpy(buff, &Ram.disk[bpos], RM_BLKSIZ);

    if (POSITION < RM_BLKS-1) {
        POSITION += 1;
    } else {
        POSITION = 0;
    }

    if (blk <= DIST) {
        TOTALCOST += blk * SMALLCOST;
    } else {
        TOTALCOST += blk * LARGECOST;
    }

	return OK;
}
