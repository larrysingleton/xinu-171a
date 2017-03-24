/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

#include <xinu.h>

struct leddevice ldev;

int32 ledinit (void)
{
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

    return (devcall)OK;
}

