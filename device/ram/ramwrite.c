/* ramwrite.c  -  ramwrite */

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
 * ramwrite  -  Write a block to a ram disk
 *------------------------------------------------------------------------
 */
devcall	ramwrite (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer containing a block	*/
	  int32	blk			/* Block number to write	*/
	)
{
	int32	bpos;			/* Byte position of blk		*/

	bpos = RM_BLKSIZ * blk;
	memcpy(&Ram.disk[bpos], buff, RM_BLKSIZ);

    if (POSITION < RM_BLKS-1) {
        POSITION += 1;
    } else {
        POSITION = 0;
    }

    int32 distance = abs(POSITION - blk);
    if (distance <= DIST) {
        TOTALCOST += distance * SMALLCOST;
    } else {
        TOTALCOST += distance * LARGECOST;
    }

    if (distance > 0 ) {
        if (POSITION < RM_BLKS-1) {
            POSITION = blk + 1;
        } else {
            POSITION = 0;
        }
    } else {
        ++POSITION;
    }

    return OK;
}
