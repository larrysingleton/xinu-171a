/* ledread.c - ledread */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledread  -  Read the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledread(
        did32 device,           /* device Id to use */
        int32 n                 /* Number of characaters asked to read */
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

    /* only 1 character allowed */
    if (n!=1) {
        return (devcall)SYSERR;
    }

    /* return the illuminated flag */
    return (devcall)ldev.illuminated;
}
