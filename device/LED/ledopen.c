/* ledopen.c - ledopen */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledopen  -  Open the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledopen(
        struct dentry *devptr  /* Entry in device switch table */
        )
{

    /* if the device is already open - return SYSERR */
    if (ldev.status!=LED_CLOSE) {
        return (devcall)SYSERR;
    }

    ldev.status=LED_OPEN; /* set the device status to open */
    return (devcall)devptr->dvnum;
}
