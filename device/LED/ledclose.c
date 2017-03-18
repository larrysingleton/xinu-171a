/* ledclose.c - ledclose */

#include <xinu.h>
#include <gpio_i2c.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/


/*------------------------------------------------------------------------
 * ledclose  -  Close the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledclose(
        struct dentry *devptr  /* Entry in device switch table */
        )
{

    /* if the device is not open - return SYSERR */
    if (ldev.status!=LED_CLOSE) {
        return SYSERR;
    }

    /* make sure LED is not illuminated */
    vGalileoBlinkLEDUsingLegacyGPIO(0);

    /* set illuminated flag to OFF */
    ldev.illuminated=LED_OFF;

    /* set the device status to not opened */
    ldev.status=LED_CLOSE; 

    /* reurn OK */
    return OK;
}

