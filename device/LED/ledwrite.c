/* ledwrite.c - ledwrite */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledwrite  -  Write to the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledwrite(
        struct dentry *devptr,  /* Entry in device switch table */
        char *buff,             /* Character to write */
        int32 n                 /* Number of characaters asked to write */
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

    /* only allowed to write LED_ON or LED_OFF */
    if (strcmp(buff, LED_ON)!=0 && strcmp(buff, LED_OFF)!=0) {
        return SYSERR;
    }

    /* set the flag */
    ldev.illuminated = buff;

    /* return OK */
    return OK;
}
