/* ledclose.c - ledclose */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/


/*------------------------------------------------------------------------
 * ledclose  -  Close the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledclose(
        int32   device  /* Device ID to use */
        )
{

    /* check for a bad device */
    if (isbaddev(device) || device != LED) {
        return (devcall)SYSERR;
    }

    /* if the device is not open - return SYSERR */
    if (ldev.status != LED_OPEN) {
     return (devcall)SYSERR;
    }

    /* make sure LED is not illuminated */
    vGalileoBlinkLEDUsingLegacyGPIO(0);

    /* set illuminated flag to OFF */
    ldev.illuminated=LED_OFF;

    /* set the device status to not opened */
    ldev.status=LED_CLOSE; 

    /* reurn OK */
    return (devcall)OK;
}

