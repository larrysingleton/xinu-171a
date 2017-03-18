#include <xinu.h>

static void doblink(uint32 state)
{
    vGalileoBlinkLEDUsingLegacyGPIO(state);
}

static void usage(void)
{
    printf("Usage:\n");
    printf("   blink --help\n");
    printf("or\n");
    printf("   blink [-p nmsec] nblinks\n");
    printf("\n");
    printf("Blink the GP LED on the Galileo Gen 2 board.\n");
    printf("N is the number of on/off cycles desired (at least 1).\n");
    printf("By default, the cycle is 500 msec on, then 500 msec off.\n");
    printf("This can be changed by specified the period, in milliseconds\n");
    printf("with the -p nmsec option, where 1 <= nmsec <= 10000\n");
    exit();
}

shellcmd xsh_blink(int nargs, char *args[])
{
    int n, msec;

    if (nargs == 2 && strcmp(args[1],"--help") == 0)
	usage();

    if (nargs != 2 && nargs != 4)
	usage();

    if (nargs == 4) {
	if (strcmp(args[1],"-p") != 0)
	    usage();
	msec = atoi(args[2]);
	if (msec < 1 || msec > 10000)
	    usage();
	n = atoi(args[3]);
    } else {
	msec = 500;
	n = atoi(args[1]);
    }
    if (n < 1)
	usage();

    /*--------------------------*/
    /* Initialize the hardware. */
    /*--------------------------*/
    vGalileoInitializeGpioController();

    vGalileoInitializeLegacyGPIO();

    vGalileoLegacyGPIOInitializationForLED();

    /*------------------*/
    /* Do the blinking. */
    /*------------------*/
    while (n > 0) {
	doblink(1);		/* illuminate the LED */
	sleepms(msec);
	doblink(0);		/* extinguish the LED */
	sleepms(msec);
	n--;
    }
    return 0;
}
