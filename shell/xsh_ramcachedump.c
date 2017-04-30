#include <xinu.h>
#include <stdlib.h>
#include <ramdisk.h>

shellcmd xsh_ramcachedump (int nargs, char *args[]) {
    printf("Ram cache\n");
    printf("Index\tBlockNo\tNext\n");
    for(int i = 0; i< RM_CACHE; i++) {
        printf("%d\t%d\t%d\n", i, RamCache[i].blockno, RamCache[i].next);
    }
    printf("RamHead: %d RamZero: %d\n", RamCacheHead, RamCacheFree);

    return OK;
}
