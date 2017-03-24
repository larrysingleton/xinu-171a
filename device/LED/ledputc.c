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
        char value
        )
{

    /* if the device is closed - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return (devcall)SYSERR;
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
        return (devcall)SYSERR;
    }

    /* set the flag */
    ldev.illuminated = value;

    /* return OK */
    return (devcall)OK;
}
