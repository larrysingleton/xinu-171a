/*------------*/
/* gpio_i2c.h */
/*------------*/
#ifndef __GPIO_I2C_H__
#define __GPIO_I2C_H__

#include "galileo_gen_defs.h"

/*----------------------------------------*/
/* PCI Configuration Map Register Offsets */
/*----------------------------------------*/
#define PCI_REG_VID             0x00    /* Vendor ID Register */
#define PCI_REG_DID             0x02    /* Device ID Register */
#define PCI_REG_PCICMD          0x04    /* PCI Command Register */
#define PCI_REG_PCISTS          0x06    /* PCI Status Register */
#define PCI_REG_RID             0x08    /* PCI Revision ID Register */
#define PCI_REG_PI              0x09    /* Programming Interface */
#define PCI_REG_SCC             0x0a    /* Sub Class Code Register */
#define PCI_REG_BCC             0x0b    /* Base Class Code Register */
#define PCI_REG_PMLT            0x0d    /* Primary Master Latency Timer */
#define PCI_REG_HDR             0x0e    /* Header Type Register */
#define PCI_REG_PBUS            0x18    /* Primary Bus Number Register */
#define PCI_REG_SBUS            0x19    /* Secondary Bus Number Register */
#define PCI_REG_SUBUS           0x1a    /* Subordinate Bus Number Register */
#define PCI_REG_SMLT            0x1b    /* Secondary Master Latency Timer */
#define PCI_REG_IOBASE          0x1c    /* I/O base Register */
#define PCI_REG_IOLIMIT         0x1d    /* I/O Limit Register */
#define PCI_REG_SECSTATUS       0x1e    /* Secondary Status Register */
#define PCI_REG_MEMBASE         0x20    /* Memory Base Register */
#define PCI_REG_MEMLIMIT        0x22    /* Memory Limit Register */
#define PCI_REG_PRE_MEMBASE     0x24    /* Prefetchable memory Base register */
#define PCI_REG_PRE_MEMLIMIT    0x26    /* Prefetchable memory Limit register */
#define PCI_REG_SVID0           0x2c    /* Subsystem Vendor ID low byte */
#define PCI_REG_SVID1           0x2d    /* Subsystem Vendor ID high byte */
#define PCI_REG_SID0            0x2e    /* Subsystem ID low byte */
#define PCI_REG_SID1            0x2f    /* Subsystem ID high byte */
#define PCI_REG_IOBASE_U        0x30    /* I/O base Upper Register */
#define PCI_REG_IOLIMIT_U       0x32    /* I/O Limit Upper Register */
#define PCI_REG_INTLINE         0x3c    /* Interrupt Line Register */
#define PCI_REG_BRIDGE_CNTL     0x3e    /* Bridge Control Register */

#define IO_PCI_ADDRESS(bus, dev, fn, reg) \
    (0x80000000 | (bus << 16) | (dev << 11) | (fn << 8) | (reg & ~3))

/*------------------------*/
/* PCI Read/Write IO Data */
/*------------------------*/
#define IO_PCI_ADDRESS_PORT 0xcf8
#define IO_PCI_DATA_PORT 0xcfc

/*-----------------*/
/* GPIO structures */
/*-----------------*/

struct __attribute__ ((__packed__)) BOARD_GPIO_CONTROLLER_CONFIG
{
    uint32  PortADR;		/* Value for IOH REG GPIO_SWPORTA_DR. */
    uint32  PortADir;		/* Value for IOH REG GPIO_SWPORTA_DDR. */
    uint32  IntEn;		/* Value for IOH REG GPIO_INTEN. */
    uint32  IntMask;		/* Value for IOH REG GPIO_INTMASK. */
    uint32  IntType;		/* Value for IOH REG GPIO_INTTYPE_LEVEL. */
    uint32  IntPolarity;	/* Value for IOH REG GPIO_INT_POLARITY. */
    uint32  Debounce;		/* Value for IOH REG GPIO_DEBOUNCE. */
    uint32  LsSync;		/* Value for IOH REG GPIO_LS_SYNC. */
};

/*-----------------------------------------*/
/* Fields are values for QNC NC Registers. */
/*-----------------------------------------*/
struct  __attribute__ ((__packed__)) BOARD_LEGACY_GPIO_CONFIG
{
    uint32 CoreWellEnable;                /* R_QNC_GPIO_CGEN_CORE_WELL. */
    uint32 CoreWellIoSelect;              /* R_QNC_GPIO_CGIO_CORE_WELL. */
    uint32 CoreWellLvlForInputOrOutput;   /* R_QNC_GPIO_CGLVL_CORE_WELL. */
    uint32 CoreWellTriggerPositiveEdge;   /* R_QNC_GPIO_CGTPE_CORE_WELL. */
    uint32 CoreWellTriggerNegativeEdge;   /* R_QNC_GPIO_CGTNE_CORE_WELL. */
    uint32 CoreWellGPEEnable;             /* R_QNC_GPIO_CGGPE_CORE_WELL. */
    uint32 CoreWellSMIEnable;             /* R_QNC_GPIO_CGSMI_CORE_WELL. */
    uint32 CoreWellTriggerStatus;         /* R_QNC_GPIO_CGTS_CORE_WELL. */
    uint32 CoreWellNMIEnable;             /* R_QNC_GPIO_CGNMIEN_CORE_WELL. */
    uint32 ResumeWellEnable;              /* R_QNC_GPIO_RGEN_RESUME_WELL. */
    uint32 ResumeWellIoSelect;            /* R_QNC_GPIO_RGIO_RESUME_WELL. */
    uint32 ResumeWellLvlForInputOrOutput; /* R_QNC_GPIO_RGLVL_RESUME_WELL. */
    uint32 ResumeWellTriggerPositiveEdge; /* R_QNC_GPIO_RGTPE_RESUME_WELL. */
    uint32 ResumeWellTriggerNegativeEdge; /* R_QNC_GPIO_RGTNE_RESUME_WELL. */
    uint32 ResumeWellGPEEnable;           /* R_QNC_GPIO_RGGPE_RESUME_WELL. */
    uint32 ResumeWellSMIEnable;           /* R_QNC_GPIO_RGSMI_RESUME_WELL. */
    uint32 ResumeWellTriggerStatus;       /* R_QNC_GPIO_RGTS_RESUME_WELL. */
    uint32 ResumeWellNMIEnable;           /* R_QNC_GPIO_RGNMIEN_RESUME_WELL.*/
} ;

/*------------------*/
/* GPIO definitions */
/*------------------*/
#define GALILEO_GEN2_GPIO_CONTROLLER_INITIALIZER \
    {0x05, 0x05, 0, 0, 0, 0, 0, 0}

#define GALILEO_GEN2_LEGACY_GPIO_INITIALIZER \
    {0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x03, 0x00, 0x3f, 0x1c, 0x02, 0x00, 0x00, \
    0x00, 0x00, 0x3f, 0x00}

#define PLATFORM_GPIO_CONTROLLER_CONFIG_DEFINITION \
/* EFI_PLATFORM_TYPE - Galileo Gen 2 */   \
GALILEO_GEN2_GPIO_CONTROLLER_INITIALIZER ,\

#define PLATFORM_LEGACY_GPIO_CONFIG_DEFINITION \
/* EFI_PLATFORM_TYPE - Galileo Gen 2 */   \
GALILEO_GEN2_LEGACY_GPIO_INITIALIZER ,    \

#define IOH_I2C_GPIO_BUS_NUMBER           0x00
#define IOH_I2C_GPIO_DEVICE_NUMBER        0x15
#define IOH_I2C_GPIO_FUNCTION_NUMBER      0x02

#define INTEL_VENDOR_ID 		  0x8086
#define V_IOH_I2C_GPIO_VENDOR_ID          INTEL_VENDOR_ID
#define V_IOH_I2C_GPIO_DEVICE_ID          0x0934

#define R_IOH_I2C_MEMBAR                  0x10
#define R_IOH_GPIO_MEMBAR                 0x14

#define GPIO_SWPORTA_DR                   0x00
#define GPIO_SWPORTA_DDR                  0x04
#define GPIO_SWPORTB_DR                   0x0C
#define GPIO_SWPORTB_DDR                  0x10
#define GPIO_SWPORTC_DR                   0x18
#define GPIO_SWPORTC_DDR                  0x1C
#define GPIO_SWPORTD_DR                   0x24
#define GPIO_SWPORTD_DDR                  0x28
#define GPIO_INTEN                        0x30
#define GPIO_INTMASK                      0x34
#define GPIO_INTTYPE_LEVEL                0x38
#define GPIO_INT_POLARITY                 0x3C
#define GPIO_INTSTATUS                    0x40
#define GPIO_RAW_INTSTATUS                0x44
#define GPIO_DEBOUNCE                     0x48
#define GPIO_PORTA_EOI                    0x4C
#define GPIO_EXT_PORTA                    0x50
#define GPIO_EXT_PORTB                    0x54
#define GPIO_EXT_PORTC                    0x58
#define GPIO_EXT_PORTD                    0x5C
#define GPIO_LS_SYNC                      0x60
#define GPIO_CONFIG_REG2                  0x70
#define GPIO_CONFIG_REG1                  0x74

/*-------------------------------*/
/* GPIO defines for cypress chip */
/*-------------------------------*/
#define PCAL9555_REG_OUT_PORT0            0x02
#define PCAL9555_REG_OUT_PORT1            0x03
#define PCAL9555_REG_CFG_PORT0            0x06
#define PCAL9555_REG_CFG_PORT1            0x07
#define PCAL9555_REG_PULL_EN_PORT0        0x46
#define PCAL9555_REG_PULL_EN_PORT1        0x47
#define PCAL9555_REG_PULL_PORT0           0x48
#define PCAL9555_REG_PULL_PORT1           0x49

/*--------------------------------------------------------*/
/* Three IO Expanders at fixed addresses on Galileo Gen2. */
/*--------------------------------------------------------*/
#define GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR	0x25
#define GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR	0x26
#define GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR	0x27

/*------------------------------------*/
/* PWM CHIP (PCA9685) on Galileo Gen2 */
/*------------------------------------*/
#define PCA9685_7BIT_SLAVE_ADDR			0x47

/*---------------------*/
/* Legacy GPIO defines */
/*---------------------*/
#define LEGACY_GPIO_BUS_NUMBER 		0
#define LEGACY_GPIO_DEVICE_NUMBER 	31
#define LEGACY_GPIO_FUNCTION_NUMBER 	0

#define R_QNC_LPC_GBA_BASE           	0x44
#define B_QNC_LPC_GPA_BASE_MASK       	0x0000FFC0

/*--------------------------*/
/* I2C structures and enums */
/*--------------------------*/
typedef struct
{
    /*-------------------------------------------*/
    /* The I2C hardware address to which the I2C */
    /* device is preassigned or allocated.       */
    /*-------------------------------------------*/
    uintn I2CDeviceAddress : 10;
} I2C_DEVICE_ADDRESS;

typedef enum _I2C_ADDR_MODE
{
    EfiI2CSevenBitAddrMode,
    EfiI2CTenBitAddrMode,
} I2C_ADDR_MODE;

/*-----------------*/
/* I2C definitions */
/*-----------------*/
#define GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO   5
#define GALILEO_GEN2_RESUMEWELL_GPIO_IO9	2
#define R_QNC_GPIO_CGEN_CORE_WELL       0x00
#define R_QNC_GPIO_CGIO_CORE_WELL       0x04
#define R_QNC_GPIO_CGLVL_CORE_WELL      0x08
#define R_QNC_GPIO_CGTPE_CORE_WELL      0x0C   // Core well GPIO Trigger Positive Edge Enable
#define R_QNC_GPIO_CGTNE_CORE_WELL      0x10   // Core well GPIO Trigger Negative Edge Enable
#define R_QNC_GPIO_CGGPE_CORE_WELL      0x14   // Core well GPIO GPE Enable
#define R_QNC_GPIO_CGSMI_CORE_WELL      0x18   // Core well GPIO SMI Enable
#define R_QNC_GPIO_CGTS_CORE_WELL       0x1C   // Core well GPIO Trigger Status
#define R_QNC_GPIO_RGEN_RESUME_WELL     0x20
#define R_QNC_GPIO_RGIO_RESUME_WELL     0x24
#define R_QNC_GPIO_RGLVL_RESUME_WELL    0x28
#define R_QNC_GPIO_RGTPE_RESUME_WELL    0x2C   // Resume well GPIO Trigger Positive Edge Enable
#define R_QNC_GPIO_RGTNE_RESUME_WELL    0x30   // Resume well GPIO Trigger Negative Edge Enable
#define R_QNC_GPIO_RGGPE_RESUME_WELL    0x34   // Resume well GPIO GPE Enable
#define R_QNC_GPIO_RGSMI_RESUME_WELL    0x38   // Resume well GPIO SMI Enable
#define R_QNC_GPIO_RGTS_RESUME_WELL     0x3C   // Resume well GPIO Trigger Status
#define R_QNC_GPIO_CNMIEN_CORE_WELL     0x40   // Core well GPIO NMI Enable
#define R_QNC_GPIO_RNMIEN_RESUME_WELL   0x44   // Resume well GPIO NMI Enable

#define B_IOH_I2C_GPIO_MEMBAR_ADDR_MASK    0xFFFFF000   // [31:12].
#define I2C_REG_CLR_START_DET              0x64         // Clear START DET Interrupt Register
#define I2C_REG_CLR_STOP_DET               0x60         // Clear STOP DET Interrupt Register
#define B_I2C_REG_CLR_START_DET            (BIT0)       // Clear START DET Interrupt Register
#define B_I2C_REG_CLR_STOP_DET             (BIT0)       // Clear STOP DET Interrupt Register
#define B_I2C_REG_CON_10BITADD_MASTER      (BIT4)       // 7-bit addressing (0) or 10-bit addressing (1)
#define B_I2C_REG_CON_SPEED                (BIT2+BIT1)  // standard mode (01) or fast mode (10)
#define I2C_REG_CON                        0x00         // Control Register
#define I2C_REG_ENABLE                     0x6C         // Enable Register
#define B_I2C_REG_ENABLE                   (BIT0)       // Enable (1) or disable (0) I2C Controller
#define I2C_REG_ENABLE_STATUS              0x9C         // Enable Status Register
#define I2C_REG_CLR_INT                    0x40         // Clear Combined and Individual Interrupt Register
#define MAX_T_POLL_COUNT       		   100
#define TI2C_POLL                	   25           // microseconds
#define I2C_REG_CLR_RX_OVER                0x48         // Clear RX Over Interrupt Register
#define I2C_REG_CLR_TX_OVER                0x4C         // Clear TX Over Interrupt Register
#define I2C_REG_CLR_TX_ABRT                0x54         // Clear TX ABRT Interrupt Register
#define I2C_FIFO_SIZE                      16
#define I2C_REG_TAR                        0x04          // Master Target Address Register
#define B_I2C_REG_TAR                      (BIT9+BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0) // Master Target Address bits
#define I2C_REG_DATA_CMD                   0x10          // Data Buffer and Command Register
#define B_I2C_REG_DATA_CMD_RW              (BIT8)        // Data Buffer and Command Register Read/Write bit
#define I2C_REG_RXFLR                      0x78          // Receive FIFO Level Register
#define B_I2C_REG_DATA_CMD_STOP            (BIT9)        // Data Buffer and Command Register STOP bit
#define I2C_REG_RAW_INTR_STAT              0x34          // Raw Interrupt Status Register
#define I2C_REG_RAW_INTR_STAT_RX_OVER      (BIT1)        // Raw Interrupt Status Register RX Overflow signal status.
#define I2C_REG_RAW_INTR_STAT_RX_UNDER     (BIT0)        // Raw Interrupt Status Register RX Underflow signal status.
#define I2C_REG_CLR_RX_UNDER               0x44          // Clear RX Under Interrupt Register
#define MAX_STOP_DET_POLL_COUNT 	   ((1000 * 1000) / TI2C_POLL)  // Extreme for expected Stop detect.
#define I2C_REG_RAW_INTR_STAT_TX_ABRT      (BIT6)        // Raw Interrupt Status Register TX Abort status.
#define I2C_REG_RAW_INTR_STAT_TX_OVER      (BIT3)        // Raw Interrupt Status Register TX Overflow signal status.
#define I2C_REG_RAW_INTR_STAT_STOP_DET     (BIT9)        // Raw Interrupt Status Register STOP_DET signal status.

/*-----------------*/
/* GPIO Prototypes */
/*-----------------*/
#define GPIO_OUTPUT	(0)
#define GPIO_INPUT 	(1)
#define LOW		(0)
#define HIGH 		(1)
#define GPIO_NUMBER     (7UL)

void vMicroSecondDelay(uint32 DelayTime);
void vMilliSecondDelay(uint32 DelayTime);
void vGalileoInitializeLegacyGPIO(void);
void vGalileoInitializeGpioController(void);
void vGalileoLegacyGPIOInitializationForLED(void);
void vGalileoLegacyGPIOInitializationForIO9(void);
void vGalileoSetGPIOBitDirection(uint32 GPIONumber, uint32 Direction);
void vGalileoSetGPIOBitLevel(uint32 GPIONumber, uint32 Level);
void vGalileoBlinkLEDUsingLegacyGPIO(uint32 Level);

#endif /* __GPIO_I2C_H__ */
