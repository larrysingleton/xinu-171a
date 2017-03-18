/* ledread.c - ledread */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledread  -  Read the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledread(
        struct dentry *devptr,  /* Entry in device switch table */
        char *buff,             /* Character read */
        int32 n                 /* Number of characaters asked to read */
        )
{

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return SYSERR;
    }

    /* only 1 character allowed */
    if (n!=1) {
        return SYSERR;
    }

    /* return the illuminated flag */
    return (devcall)ldev.illuminated;
}
