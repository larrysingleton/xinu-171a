/* led.h - constants and structure for led device type */

#define LED_ON 'Y'
#define LED_OFF 'N'

#define LED_OPEN 1
#define LED_CLOSE 0

struct leddevice {
    short status;          /* 0 if not open, non-zero otherwise */
    char illuminated;    /* Y if illuminated, N if not */
};
