/* ledgetc.c - ledgetc */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledgetc  -  Read the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledgetc(
        struct dentry *devptr   /* device Id to use */
        )
{

     /* check for a bad device */
     if (isbaddev(devptr->dvnum) || devptr->dvnum != LED) {
         return SYSERR;
     }

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return SYSERR;
    }

    /* return the illuminated flag */
    return ldev.illuminated;
}
