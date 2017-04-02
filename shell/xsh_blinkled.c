/*
 * CSCI-8530 - Programming Assignment 2
 * Authors:
 *      Larry Singleton & Zac McFarland
 *
 *
*/

#include <xinu.h>
#include <stdlib.h>

int ledcheck(void);

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

/*-------------------------------------------------------------------------*/
/* Verify, to the extent possible, the proper operation of the LED device. */
/* If an incorrect result is detected, display a message and return 0.     */
/* Otherwise return 1.                                                     */
/*-------------------------------------------------------------------------*/
/* Note that the type "devcall" is the same as "int32"; see kernel.h.      */
/* Also note that OK is defined as 1, the normal system call return.       */
/* The write and read system calls return the number of bytes written or   */
/* read, so OK would be appropriate for the result of these.               */
/* There is a difference between the result of putc in Xinu and putc in    */
/* standard C. In Xinu, putc returns OK on success. In standard C, putc    */
/* returns the character written on success. We check for OK in this code. */
/*-------------------------------------------------------------------------*/
static int check(void) {
    int32 descr;				/* valid descriptor */
    int32 edescr;				/* invalid descriptor */
    int32 result;				/* result of I/O */
    char buff[10];				/* data buffer */

    /*-------------------------------*/
    /* Test open and close behavior. */
    /*-------------------------------*/
    descr = open(LED,"unused","unused");	/* Try to open */
    if (descr == SYSERR) {
        printf("Cannot open LED.\n");
        return 0;
    }
    edescr = open(LED,"unused","unused");	/* Try to open again */
    if (edescr != SYSERR) {
        printf("Opening LED that was already open erroneously succeeded.\n");
        return 0;
    }
    if (close(descr) != OK) {			/* Try to close */
        printf("Cannot close LED.\n");
        return 0;
    }
    if (close(descr) != SYSERR) {		/* Try to close again */
        printf("Close of already closed LED erroneously succeeded.\n");
        return 0;
    }

    /*------------------------------------------*/
    /* Now try some I/O on a closed LED device. */
    /*------------------------------------------*/
    result = putc(descr,'Y');			/* putc to closed device */
    if (result != SYSERR) {
        printf("putc to closed device erroneously succeeded.\n");
        return 0;
    }
    result = write(descr,"Y",1);		/* write closed device */
    if (result != SYSERR) {
        printf("write to closed device erroneously succeeded.\n");
        return 0;
    }
    result = getc(descr);			/* getc */
    if (result != SYSERR) {
        printf("getc to closed device erroneously succeeded.\n");
        return 0;
    }
    result = read(descr,buff,1);		/* read closed device */
    if (result != SYSERR) {
        printf("read from closed device erroneously succeeded.\n");
        return 0;
    }

    /*----------------------*/
    /* Open the LED device. */
    /*----------------------*/
    descr = open(LED,"unused","unused");	/* Try to open again */
    if (descr == SYSERR) {
        printf("Cannot open LED the second time.\n");
        return 0;
    }

    /*--------------------------------------*/
    /* Verify the initial LED state is off. */
    /*--------------------------------------*/
    result = getc(descr);			/* getc */
    if (result == SYSERR) {
        printf("getc on open device failed when it should have succeeded.\n");
        return 0;
    }
    if (result != 'N') {
        printf("getc succeeded but returned %c; it should have returned 'N'\n");
        return 0;
    }

    /*-------------------------------------------*/
    /* Now try normal I/O, and a few bad values. */
    /*-------------------------------------------*/
    result = putc(descr,'Y');			/* putc */
    if (result != OK) {
        printf("putc 'Y' failed when it should have succeeded.\n");
        return 0;
    }
    result = putc(descr,'N');			/* putc */
    if (result != OK) {
        printf("putc 'N' failed when it should have succeeded.\n");
        return 0;
    }
    result = putc(descr,'X');			/* putc */
    if (result != SYSERR) {
        printf("putc 'X' succeeded when it should have failed.\n");
        return 0;
    }
    buff[0] = 'Y';
    result = write(descr,buff,1);		/* write */
    if (result == SYSERR) {
        printf("write 'Y' failed when it should have succeeded.\n");
        return 0;
    }
    buff[0] = 'N';
    result = write(descr,buff,1);		/* write */
    if (result == SYSERR) {
        printf("write 'N' failed when it should have succeeded.\n");
        return 0;
    }
    buff[0] = 'Y';
    buff[1] = 'N';
    result = write(descr,buff,2);		/* write */
    if (result != SYSERR) {
        printf("write \"YN\" succeeded when it should have failed.\n");
        return 0;
    }
    buff[0] = 'Z';
    result = write(descr,buff,1);		/* write */
    if (result != SYSERR) {
        printf("write 'Z' succeeded when it should have failed.\n");
        return 0;
    }
    result = getc(descr);			/* getc */
    if (result == SYSERR) {
        printf("getc on open device failed when it should have succeeded.\n");
        return 0;
    }
    result = read(descr,buff,2);		/* try to read too much */
    if (result != SYSERR) {
        printf("Attempt to read 2 bytes succeeded, but should have failed.\n");
        return 0;
    }
    putc(descr,'Y');
    result = getc(descr);
    if (result != 'Y') {
        printf("getc returned %c, but should have returned 'Y'\n", result);
        return 0;
    }

    close(descr);				/* finish up */
    return 1;
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

    if(check() == 1) {
        /* Open device */
        int32 descr = open(LED, "unused","unused");
        /* Do the blinking. */
        while (n > 0) {
            putc(descr,'Y');		/* illuminate the LED */
            sleepms(msec);
            putc(descr, 'N');		/* extinguish the LED */
            sleepms(msec);
            n--;
            printf(".");
        }
        /* Close device */
        close(descr);

        return 0;
    } else {
        printf("\nFailed to blink, device configuration invalid\n");
        return SYSERR;
    }
}