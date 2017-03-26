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
        did32 device    /* device Id to use */
        )
{

     /* check for a bad device */
     if (isbaddev(device) || device != LED0) {
         return (devcall)SYSERR;
     }

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return (devcall)SYSERR;
    }

    /* return the illuminated flag */
    return (devcall)ldev.illuminated;
}
