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

devcall ledopen( void )
{

    /* check to ensure device is good */
    if (isbaddev(LED)) {
        return (devcall)SYSERR;
    }

    /* if the device is already open - return SYSERR */
    if (ldev.status!=LED_CLOSE) {
        return (devcall)SYSERR;
    }

    /* set the device status to open */
    ldev.status=LED_OPEN; 

    /* return the device id */
    return (devcall)LED;
}
