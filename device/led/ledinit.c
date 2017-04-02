/* ledinit.c - ledinit */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/


struct leddevice ldev;

status ledinit (
        struct dentry *devptr /*device pointer */
        )
{

    /* check to ensure device is good */
     if (isbaddev(devptr->dvnum) || devptr->dvnum != LED) {
         return (status) SYSERR;
     }
 
    /*--------------------------*/
    /* Initialize the datastructure. */
    /*--------------------------*/
    ldev.status = LED_CLOSE;
    ldev.illuminated = LED_OFF;

    /*--------------------------*/
    /* Initialize the hardware. */
    /*--------------------------*/
    vGalileoInitializeGpioController();
    vGalileoInitializeLegacyGPIO();
    vGalileoLegacyGPIOInitializationForLED();

    /* return OK */
    return (status) OK;
}

