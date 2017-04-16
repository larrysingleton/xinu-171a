/* raminit.c  -  raminit */

/*
 * CSCI-8530 - Programming Assignment 3
 * Authors:
 *      Larry Singleton & Zac McFarland
 */


#include <xinu.h>
#include <ramdisk.h>

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

int32 DIST;
int32 SMALLCOST;
int32 LARGECOST;
int32 TOTALCOST;
int32 POSITION;

devcall	raminit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	memcpy(Ram.disk, "hopeless", 8);
	memcpy( &Ram.disk[8], Ram.disk, RM_BLKSIZ * RM_BLKS - 8);

	DIST = 0;
	SMALLCOST = 0;
	LARGECOST = 1;
    TOTALCOST = 0;
    POSITION = 0;

	return OK;
}
