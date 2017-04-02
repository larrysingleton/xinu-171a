/* ledwrite.c - ledwrite */

#include <xinu.h>

/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

/*------------------------------------------------------------------------
 * ledwrite  -  Write to the led "device"
 *------------------------------------------------------------------------
 */

extern struct leddevice ldev;

devcall ledwrite(
        struct dentry *devptr,  /* device Id to use */
        char *buff,             /* Character to write */
        uint32 n                 /* Number of characaters asked to write */
        )
{

     /* check for a bad device */
     if (isbaddev(devptr->dvnum) || devptr->dvnum != LED) {
         return SYSERR;
     }

    /* if the device is not open - return SYSERR */
    if (ldev.status==LED_CLOSE) {
        return SYSERR;
    }

    /* only 1 character allowed */
    if (n!=1) {
        return SYSERR;
    }

    /*
     * Ensures only LED_ON and LED_OFF are allowed and
     * transitions the device appropriately.
     */
    if(*buff == LED_ON) {
        vGalileoBlinkLEDUsingLegacyGPIO(1);
    } else if (*buff == LED_OFF) {
        vGalileoBlinkLEDUsingLegacyGPIO(0);
    } else {
        return SYSERR;
    }

    /* set the flag */
    ldev.illuminated = *buff;

    /* return OK */
    return OK;
}
