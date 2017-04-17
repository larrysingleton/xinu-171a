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
#include <stdlib.h>

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
    int32   distance;

	bpos = RM_BLKSIZ * blk;
	memcpy(buff, &Ram.disk[bpos], RM_BLKSIZ);

    distance = abs(POSITION - blk);
    fprintf(stdout, "distance [%d]\n", distance);
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

    fprintf(stdout, "Position [%d]\n", POSITION);
	return OK;
}
