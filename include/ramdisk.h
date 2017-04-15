/* ramdisk.h - definitions for a ram disk */

/*---------------------*/
/* Ram disk block size */
/*---------------------*/
#define	RM_BLKSIZ 512		/* block size */
#define	RM_BLKS	200		/* number of blocks */

/*-------------------*/
/* Control functions */
/*-------------------*/
#define RAM_CTL_SET 1		/* set dist, smallcost, largecost */
#define RAM_CTL_ZERO 2		/* set total simulated seek cost to 0 */
#define RAM_CTL_READ 3		/* return total simulated seek cost */

struct	ramdisk	{
    char disk[RM_BLKSIZ * RM_BLKS];
};

extern struct ramdisk Ram;

struct ramcache {
    char block[RM_BLKSIZ];	/* a cached disk block */
    int blockno;		/* block number of cached block */
    int next;			/* index of next cached block, or -1 */
};

#define RM_CACHE 20		/* number of cached blocks */
struct ramcache RamCache[RM_CACHE];
uint32 RamCacheHead;	/* head of cache linked list, or -1 */
uint32 RamCacheFree;	/* head of cache free list, or -1 */

#define RAMDISKDEBUG 0
