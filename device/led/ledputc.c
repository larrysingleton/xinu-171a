/* ledputc.c - ledputc */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledputc  -  Write to the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledputc(
        struct dentry *device,  /* device to use */
        char value             /* value to plut */
        )
{

    /* check for a bad device */
    if (isbaddev(device->dvnum) || device->dvnum != LED) {
        return SYSERR;
    }

    /* if the device is closed - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return SYSERR;
    }

    /*
     * Ensures only LED_ON and LED_OFF are allowed and
     * transitions the device appropriately.
     */
    if(value == LED_ON) {
        vGalileoBlinkLEDUsingLegacyGPIO(1);
    } else if (value == LED_OFF) {
        vGalileoBlinkLEDUsingLegacyGPIO(0);
    } else {
        return SYSERR;
    }

    /* set the flag */
    ldev.illuminated = value;

    /* return OK */
    return OK;
}
