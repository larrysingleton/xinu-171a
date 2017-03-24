/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

#include <xinu.h>
#include <stdlib.h>

extern struct leddevice ldev;

static void usage(void) {
    printf("Usage:\n");
    printf("   blinkled --help\n");
    printf("or\n");
    printf("   blinkled [-p nmsec] nblinks\n");
    printf("\n");
    printf("Blink the GP LED on the Galileo Gen 2 board.\n");
    printf("N is the number of on/off cycles desired (at least 1).\n");
    printf("By default, the cycle is 500 msec on, then 500 msec off.\n");
    printf("This can be changed by specified the period, in milliseconds\n");
    printf("with the -p nmsec option, where 1 <= nmsec <= 10000\n");
    exit();
}

static void printIResult(int result, bool8 status) {
    if(status) {
        printf("%2d     \tOK\n", result);
    } else {
        printf("%2d     \tERROR\n", result);
    }
}

static void printCResult(char result, bool8 status) {
    if(status) {
        printf("%2c     \tOK\n", result);
    } else {
        printf("%2c     \tERROR\n", result);
    }
}

static void verifyHardwareConfiguration() {
    int transientI;
    char transientC;

    printf("Verifying LED driver configuration\n");
    printf("----------------------------------\n");
    printf("Use Case          \tResult\tStatus\n");

    printf("[Successful Open] \t");
    ledopen();
    printIResult(ldev.status, ldev.status == LED_OPEN);

    printf("[Double Open]     \t");
    transientI = ledopen();
    printIResult(transientI, transientI == SYSERR);

    printf("[Put bad char]    \t");
    transientI = ledputc('X');
    printIResult(transientI, transientI == SYSERR);

    printf("[Put char]        \t");
    ledputc(LED_ON);
    printCResult(ldev.illuminated, ldev.illuminated == LED_ON);

    printf("[Write char]      \t");
    ledputc(LED_OFF);
    ledwrite("Y", 1);
    printCResult(ldev.illuminated, ldev.illuminated == LED_ON);

    printf("[Write char YES]      \t");
    ledputc(LED_OFF);
    ledwrite("YES", 1);
    printCResult(ldev.illuminated, ldev.illuminated == LED_ON);

    printf("[Bad Write #]    \t");
    transientI = ledwrite("Y", 2);
    printIResult(transientI, transientI == SYSERR);

    printf("[Bad Write X]    \t");
    transientI = ledwrite("X", 1);
    printIResult(transientI, transientI == SYSERR);

    printf("[Bad Write STRING]    \t");
    transientI = ledwrite("STRING", 1);
    printIResult(transientI, transientI == SYSERR);

    printf("[Get char]        \t");
    transientC = ledgetc();
    printCResult(transientC, transientC == LED_ON);

    printf("[Read char]       \t");
    transientC = ledread(1);
    printCResult(transientC, transientC == LED_ON);

    printf("[Bad Read #]      \t");
    transientI = ledread(2);
    printIResult(transientI, transientI == SYSERR);

    printf("[Successful Close]\t");
    ledclose();
    printIResult(ldev.status, ldev.status == LED_CLOSE);

    printf("[Put Closed]     \t");
    transientI = ledputc(LED_ON);
    printIResult(transientI, transientI == SYSERR);

    printf("[Write Closed]    \t");
    transientI = ledwrite("Y", 1);
    printIResult(transientI, transientI == SYSERR);

    printf("[Get Closed]     \t");
    transientI = ledgetc();
    printIResult(transientI, transientI == SYSERR);

    printf("[Read Closed]     \t");
    transientI = ledread(1);
    printIResult(transientI, transientI == SYSERR);

    printf("[Double Close]    \t");
    transientI = ledclose();
    printIResult(transientI, transientI == SYSERR);

    printf("Device verification complete\n\n");
}

shellcmd xsh_blinkled(int nargs, char *args[]) {
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

    verifyHardwareConfiguration();

    /*------------------*/
    /* Do the blinking. */
    /*------------------*/
    ledopen();
    while (n > 0) {
        ledputc(LED_ON);		/* illuminate the LED */
        sleepms(msec);
        ledputc(LED_OFF);		/* extinguish the LED */
        sleepms(msec);
        n--;
        printf(".");
    }

    ledclose();
    printf("\n");
    return 0;
}
