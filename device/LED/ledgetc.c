/* ledgetc.c - ledgetc */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledgetc  -  Read the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledgetc(
        struct dentry *devptr  /* Entry in device switch table */
        )
{

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return (devcall)SYSERR;
    }

    /* return the illuminated flag */
    return (devcall)ldev.illuminated;
}
