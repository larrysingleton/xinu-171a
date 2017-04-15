/* ramdisk.h - definitions for a ram disk */

/* Ram disk block size */

#define	RM_BLKSIZ	512		/* block size			*/
#define	RM_BLKS		200		/* number of blocks		*/

#define RAM_CTL_SET   1
#define RAM_CTL_ZERO  2
#define RAM_CTL_READ  3

struct	ramdisk	{
	char	disk[RM_BLKSIZ * RM_BLKS];
	};

extern	struct	ramdisk	Ram;