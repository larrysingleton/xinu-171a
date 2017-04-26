/* ramclose.c  -  ramclose */

/*-------------------------------------------------------------------------*/
/* This needs no changes for the caching version of the RAM device driver. */
/*-------------------------------------------------------------------------*/

#include <xinu.h>

/*------------------------------------------------------------------------
 * Ramclose  -  Close a ram disk
 *------------------------------------------------------------------------
 */
devcall	ramclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	return OK;
}
