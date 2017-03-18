/* ledputc.c - ledputc */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledputc  -  Write to the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledputc(
        struct dentry *devptr,  /* Entry in device switch table */
        char value
        )
{

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return SYSERR;
    }

    /* only allowed to write LED_ON or LED_OFF */
    if (value != LED_ON && value != LED_OFF) {
        return SYSERR;
    }

    /* set the flag */
    ldev.illuminated = value;

    /* return OK */
    return OK;
}
