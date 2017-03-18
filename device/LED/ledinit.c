/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zack McFarland
 *
 *
*/


#include <xinu.h>

int32 ledinit (
        struct dentry *devptr   /* Entry in the device switch table */
        )
{

    kprintf("\n******************* Initialize LED device ********************");

    /*--------------------------*/
    /* Initialize the hardware. */
    /*--------------------------*/
    vGalileoInitializeGpioController();
    vGalileoInitializeLegacyGPIO();
    vGalileoLegacyGPIOInitializationForLED();
    
    kprintf("\n******************* Initialize LED device ********************");

    return (devcall)OK;
}

