#ifndef __GALILEO_GEN_DEFS_H__
#define __GALILEO_GEN_DEFS_H__

#include <stdarg.h>

/*-------------------------------------------------------*/
/* Prototypes (assembly language functions in startup.S) */
/*-------------------------------------------------------*/
extern void halt( void );
extern int32 inb( int32 );
extern int32 inw( int32 );
extern int32 inl( int32 );
extern int32 outb( int32, int32 );
extern int32 outw( int32, int32 );
extern int32 outl( int32, int32) ;

/*----------------*/
/* GP definitions */
/*----------------*/
#ifndef TRUE
	#define TRUE ( 1 )
#endif

#ifndef FALSE
	#define FALSE ( 0 )
#endif

#ifndef true
	#define true  TRUE
#endif

#ifndef false
	#define false FALSE
#endif

#ifndef OK
	#define OK TRUE
#endif

#ifndef NULL
	#define NULL (0)
#endif

/*---------------------------------*/
/* General bit pattern definitions */
/*---------------------------------*/
#define BIT0  0x00000001U
#define BIT1  0x00000002U
#define BIT2  0x00000004U
#define BIT3  0x00000008U
#define BIT4  0x00000010U
#define BIT5  0x00000020U
#define BIT6  0x00000040U
#define BIT7  0x00000080U
#define BIT8  0x00000100U
#define BIT9  0x00000200U

/*--------------------------*/
/* MMIO support definitions */
/*--------------------------*/
#define EC_BASE			0xE0000000	/* Base of MMConfig space */
#define MMCONFIG_BASE		EC_BASE
#define MMIO_PCI_ADDRESS(bus,dev,fn,reg) ( \
        (EC_BASE) + \
		((bus) << 20) + \
		((dev) << 15) + \
		((fn)  << 12) + \
		(reg))

/*-----------------------------------------*/
/* MMIO read/write/set/clear/modify macros */
/*-----------------------------------------*/
#define mem_read(base, offset, size) ({ \
	volatile uint32 a = (base) + (offset); \
	volatile uint64 v; \
    switch (size) { \
    case 1: \
        v = (uint8)(*((uint8 *)a)); \
        break; \
    case 2: \
        v = (uint16)(*((uint16 *)a)); \
        break; \
    case 4: \
        v = (uint32)(*((uint32 *)a)); \
        break; \
    case 8: \
        v = (uint64)(*((uint64 *)a)); \
        break; \
    default: \
        halt(); \
    } \
    v; \
})

/*-------------------------------------------------------*/
/* No cache bypass necessary -- MTRRs should handle this */
/*-------------------------------------------------------*/
#define mem_write(base, offset, size, value) { \
	volatile uint32 a = (base) + (offset); \
    switch (size) { \
    case 1: \
        *((uint8 *)a) = (uint8)(value); \
        break; \
    case 2: \
        *((uint16 *)a) = (uint16)(value); \
        break; \
    case 4: \
        *((uint32 *)a) = (uint32)(value); \
        break; \
    case 8: \
        *((uint64 *)a) = (uint64)(value); \
        break; \
    default: \
        halt(); \
    } \
}

#define mem_set(base, offset, size, smask) { \
	volatile uint32 a = (base) + (offset); \
    switch (size) { \
    case 1: \
        *((uint8 *)a) = (uint8)((*((uint8 *)a)) | (smask)); \
        break; \
    case 2: \
        *((uint16 *)a) = (uint16)((*((uint16 *)a)) | (smask)); \
        break; \
    case 4: \
        *((uint32 *)a) = (uint32)((*((uint32 *)a)) | (smask)); \
        break; \
    case 8: \
        *((uint64 *)a) = (uint64)((*((uint64 *)a)) | (smask)); \
        break; \
    } \
}

#define mem_clear(base, offset, size, cmask) { \
	volatile uint32 a = (base) + (offset); \
    switch (size) { \
    case 1: \
        *((uint8 *)a) = (uint8)((*((uint8 *)a) & ~(cmask))); \
        break; \
    case 2: \
        *((uint16 *)a) = (uint16)((*((uint16 *)a) & ~(cmask))); \
        break; \
    case 4: \
        *((uint32 *)a) = (uint32)((*((uint32 *)a) & ~(cmask))); \
        break; \
    case 8: \
        *((uint64 *)a) = (uint64)((*((uint64 *)a) & ~(cmask))); \
        break; \
    } \
}

#define mem_modify(base, offset, size, cmask, smask) { \
	volatile uint32 a = (base) + (offset); \
    switch (size) { \
    case 1: \
        *((uint8 *)a) = (uint8)((*((uint8 *)a) & ~(cmask)) | (smask)); \
        break; \
    case 2: \
        *((uint16 *)a) = (uint16)((*((uint16 *)a) & ~(cmask)) | (smask)); \
        break; \
    case 4: \
        *((uint32 *)a) = (uint32)((*((uint32 *)a) & ~(cmask)) | (smask)); \
        break; \
    case 8: \
        *((uint64 *)a) = (uint64)((*((uint64 *)a) & ~(cmask)) | (smask)); \
        break; \
    } \

#endif /* GALILEO_GEN_DEFS */

