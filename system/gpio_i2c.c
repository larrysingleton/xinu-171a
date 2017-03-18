/*------------------------------------------------------------*/
/* GALILEO GEN 2 GPIO -- LIMITED TO DEALING WITH ON-BOARD LED */
/*------------------------------------------------------------*/
#include <xinu.h>
#include <gpio_i2c.h>

static void vGalileoRouteLEDPins(void);
static void PlatformPcal9555GpioSetLevel(uint32,uint32,uint32);
static void PlatformPcal9555GpioSetDir(uint32,uint32,uint32);
static void PlatformPcal9555GpioPullup(uint32,uint32,uint32);
void vMilliSecondDelay(uint32 DelayTime);

static struct BOARD_GPIO_CONTROLLER_CONFIG GpioConfig;
static struct BOARD_LEGACY_GPIO_CONFIG LegacyGpioConfig;

static uint32 LegacyGpioBase = 0;
static uint32 IohGpioBase = 0;
static uint32 I2CGpioBase = 0;

static uint32 bGalileoGPIOInitialized = FALSE;

/*------------------------*/
/* GPIO support functions */
/*------------------------*/

/*---------------------------------------------------------*/
/* Read and return 4 bytes from the specified PCI address. */
/* That is, read 4 bytes from the configuration space.     */
/*---------------------------------------------------------*/
static uint32 pciIOread32(uint32 addr)
{
    outl(IO_PCI_ADDRESS_PORT, addr);
    uint32 data = inl(IO_PCI_DATA_PORT);
    return data;
}

/*-------------------------------------------------------*/
/* Write the given 4 bytes to the specified PCI address. */
/* That is, write 4 bytes to the configuration space.    */
/*-------------------------------------------------------*/
static void pciIOwrite32(uint32 addr, uint32 IO_data)
{
    outl(IO_PCI_ADDRESS_PORT, addr);
    outl(IO_PCI_DATA_PORT, IO_data );
}

/*--------------------------------------------------------------------*/
/* vGalileoInitializeGpioController must have been called previously. */
/*--------------------------------------------------------------------*/
/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static int32 uiGalileoGPIORead(uint32 Offset, uint8 UseMask)
{
    /*---------------------------*/
    /* Keep reserved bits [31:8] */
    /*---------------------------*/
    if (UseMask)
	return *((volatile uint32 *) (uintn)(IohGpioBase + Offset))
	    & 0xFFFFFF00;
    else
	return *((volatile uint32 *) (uintn)(IohGpioBase + Offset));
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void vGalileoGPIOWrite(uint32 Offset, uint32 WriteData32)
{
    uint32 Data32 = uiGalileoGPIORead(Offset, true);

    if (Offset !=  GPIO_INTSTATUS)
	Data32 |= (WriteData32 & 0x000FFFFF);
    *((volatile uint32 *) (uintn)(IohGpioBase + Offset)) = Data32;
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static int32 uiGalileoLegacyGPIOPCIRead(uint32 addr, uint32 Mask)
{
    /*----------------------------------*/
    /* Keep reserved bits (Mask Varies) */
    /*----------------------------------*/
    return pciIOread32(addr) & Mask;
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void vGalileoLegacyGPIOPCIWrite(
    uint32 addr,
    uint32 WriteData32,
    uint32 Mask)
{
    uint32 Data32 = uiGalileoLegacyGPIOPCIRead(addr, Mask);
    Data32 |= (WriteData32 & ~Mask);
    pciIOwrite32(addr, Data32);
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static int32 uiGalileoLegacyGPIOPortRead(
    uint32 addr,
    uint32 Mask)
{
    /*----------------------------------*/
    /* Keep reserved bits (Mask Varies) */
    /*----------------------------------*/
    return inl(addr) & Mask;
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void vGalileoLegacyGPIOPortRMW(
    uint32 addr,
    uint32 WriteData32,
    uint32 Mask)
{
    uint32 Data32 = uiGalileoLegacyGPIOPortRead(addr, Mask);
    Data32 |= (WriteData32 & ~Mask);
    outl(addr, Data32);
}

/*-------------------------------------*/
/* Controller initialization functions */
/*-------------------------------------*/
void vGalileoInitializeLegacyGPIO(void)
{
    /*---------------------------------------------*/
    /* Read Register Default Values into Structure */
    /*---------------------------------------------*/
    struct BOARD_LEGACY_GPIO_CONFIG LegacyGPIOConfigTable[] =
	{ PLATFORM_LEGACY_GPIO_CONFIG_DEFINITION };

    /*------------------------------------------------*/
    /* BDF for Legacy GPIO (from the Quark Datasheet) */
    /*------------------------------------------------*/
    uint8 Bus = LEGACY_GPIO_BUS_NUMBER;
    uint8 Device = LEGACY_GPIO_DEVICE_NUMBER;
    uint8 Func = LEGACY_GPIO_FUNCTION_NUMBER;

    intmask mask = disable();

    /*----------------------------------*/
    /* Get PCI Configuration IO Address */
    /*----------------------------------*/
    LegacyGpioBase =
	uiGalileoLegacyGPIOPCIRead(IO_PCI_ADDRESS(Bus, Device, Func,
	    R_QNC_LPC_GBA_BASE), B_QNC_LPC_GPA_BASE_MASK);

    /*---------------------------------------------*/
    /* Quiet compiler by doing a legacy GPIO write */
    /*---------------------------------------------*/
    uint32 PciCmd = uiGalileoLegacyGPIOPCIRead(
	(LegacyGpioBase + PCI_REG_PCICMD), 0xFFFFFFFF);
    vGalileoLegacyGPIOPCIWrite((LegacyGpioBase + PCI_REG_PCICMD),
	(PciCmd | 0x7), 0xFFFFFFFF);

    /*-----------------*/
    /* Setup Structure */
    /*-----------------*/
    LegacyGpioConfig = LegacyGPIOConfigTable[0];

    /*---------------*/
    /* Update values */
    /*---------------*/
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGEN_CORE_WELL,
	LegacyGpioConfig.CoreWellEnable, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGIO_CORE_WELL,
	LegacyGpioConfig.CoreWellIoSelect, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGLVL_CORE_WELL,
	LegacyGpioConfig.CoreWellLvlForInputOrOutput, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGTPE_CORE_WELL,
	LegacyGpioConfig.CoreWellTriggerPositiveEdge, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGTNE_CORE_WELL,
	LegacyGpioConfig.CoreWellTriggerNegativeEdge, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGGPE_CORE_WELL,
	LegacyGpioConfig.ResumeWellGPEEnable, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGSMI_CORE_WELL,
	LegacyGpioConfig.ResumeWellSMIEnable, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CGEN_CORE_WELL,
	LegacyGpioConfig.CoreWellTriggerStatus, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_CNMIEN_CORE_WELL,
	LegacyGpioConfig.ResumeWellNMIEnable, 0xFFFFFFFC);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGEN_RESUME_WELL,
	LegacyGpioConfig.ResumeWellEnable, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGIO_RESUME_WELL,
	LegacyGpioConfig.ResumeWellIoSelect, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGLVL_RESUME_WELL,
	LegacyGpioConfig.ResumeWellLvlForInputOrOutput, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGTPE_RESUME_WELL,
	LegacyGpioConfig.ResumeWellTriggerPositiveEdge, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGTNE_RESUME_WELL,
	LegacyGpioConfig.ResumeWellTriggerNegativeEdge, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGGPE_RESUME_WELL,
	LegacyGpioConfig.CoreWellGPEEnable, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGSMI_RESUME_WELL,
	LegacyGpioConfig.CoreWellSMIEnable, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RGTS_RESUME_WELL,
	LegacyGpioConfig.ResumeWellTriggerStatus, 0xFFFFFFC0);
    vGalileoLegacyGPIOPortRMW(LegacyGpioBase + R_QNC_GPIO_RNMIEN_RESUME_WELL,
	LegacyGpioConfig.CoreWellNMIEnable, 0xFFFFFFC0);

    restore(mask);
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
void vGalileoInitializeGpioController(void)
{
    /*---------------------------------------------*/
    /* Read Register Default Values into Structure */
    /*---------------------------------------------*/
    struct BOARD_GPIO_CONTROLLER_CONFIG BoardGpioControllerConfigTable[] =
	{ PLATFORM_GPIO_CONTROLLER_CONFIG_DEFINITION };

    /*---------------------------------------------------*/
    /* BDF for I2C Controller (from the Quark Datasheet) */
    /*---------------------------------------------------*/
    uint8 Bus = IOH_I2C_GPIO_BUS_NUMBER;
    uint8 Device = IOH_I2C_GPIO_DEVICE_NUMBER;
    uint8 Func = IOH_I2C_GPIO_FUNCTION_NUMBER;

    intmask mask = disable();

    /*------------------------------------*/
    /* Get PCI Configuration MMIO Address */
    /*------------------------------------*/
    uint32 gpio_controller_base = MMIO_PCI_ADDRESS(Bus, Device, Func, 0);

    /*---------------------------*/
    /* Get Vendor and Device IDs */
    /*---------------------------*/
    uint16 PciVid = mem_read(gpio_controller_base, PCI_REG_VID, 2);
    uint16 PciDid = mem_read(gpio_controller_base, PCI_REG_DID, 2);

    /*-----------------------------*/
    /* Check for valid VID and DID */
    /*-----------------------------*/
    if((PciVid == V_IOH_I2C_GPIO_VENDOR_ID)
	&& (PciDid == V_IOH_I2C_GPIO_DEVICE_ID)) {

	/*-------------*/
	/* Read PCICMD */
	/*-------------*/
	uint8 PciCmd = mem_read(gpio_controller_base, PCI_REG_PCICMD, 1);

	/*-------------------------------------------------------------*/
	/* Enable Bus Master(Bit2), MMIO Space(Bit1) & I/O Space(Bit0) */
	/*-------------------------------------------------------------*/
	mem_write(gpio_controller_base, PCI_REG_PCICMD, 1, (PciCmd | 0x7));

	/*---------------*/
	/* Read MEM_BASE */
	/*---------------*/
	IohGpioBase = mem_read(gpio_controller_base, R_IOH_GPIO_MEMBAR, 4);

	/*-----------------*/
	/* Setup Structure */
	/*-----------------*/
	GpioConfig = BoardGpioControllerConfigTable[0];

	/*--------------------------------*/
	/* IEN- Interrupt Enable Register */
	/*--------------------------------*/
	vGalileoGPIOWrite(GPIO_INTEN, GpioConfig.IntEn);

	/*------------------------------------*/
	/* ISTATUS- Interrupt Status Register */
	/*------------------------------------*/
	vGalileoGPIOWrite(GPIO_INTSTATUS, 0);

	/*----------------------------------------------*/
	/* GPIO SWPORTA Data Register - GPIO_SWPORTA_DR */
	/*----------------------------------------------*/
	vGalileoGPIOWrite(GPIO_SWPORTA_DR, GpioConfig.PortADR);

	/*---------------------------------------------------------*/
	/* GPIO SWPORTA Data Direction Register - GPIO_SWPORTA_DDR */
	/*---------------------------------------------------------*/
	vGalileoGPIOWrite(GPIO_SWPORTA_DDR, GpioConfig.PortADir);

	/*----------------------------------------*/
	/* Interrupt Mask Register - GPIO_INTMASK */
	/*----------------------------------------*/
	vGalileoGPIOWrite(GPIO_INTMASK, GpioConfig.IntMask);

	/*----------------------------------------------------*/
	/* Interrupt Level Type Register - GPIO_INTTYPE_LEVEL */
	/*----------------------------------------------------*/
	vGalileoGPIOWrite(GPIO_INTTYPE_LEVEL, GpioConfig.IntType);

	/*------------------------------------------------------*/
	/* Interrupt Polarity Type Register - GPIO_INT_POLARITY */
	/*------------------------------------------------------*/
	vGalileoGPIOWrite(GPIO_INT_POLARITY, GpioConfig.IntPolarity);

	/*--------------------------------------------------*/
	/* Interrupt Debounce Type Register - GPIO_DEBOUNCE */
	/*--------------------------------------------------*/
	vGalileoGPIOWrite(GPIO_DEBOUNCE, GpioConfig.Debounce);

	/*---------------------------------------------------------*/
	/* Interrupt Clock Synchronization Register - GPIO_LS_SYNC */
	/*---------------------------------------------------------*/
	vGalileoGPIOWrite(GPIO_LS_SYNC, GpioConfig.LsSync);
	bGalileoGPIOInitialized = true;
    }

    restore(mask);
}

/*-------------------------------------------*/
/* I/O direction and level setting functions */
/*-------------------------------------------*/

/*----------------------------------------------------------------*/
/* Sets the direction (GPIO_INPUT or GPIO_OUTPUT) for a specified */
/* GPIO number (0..7). See section 19.5.2.2 in the Quark manual.  */
/*----------------------------------------------------------------*/
void vGalileoSetGPIOBitDirection(uint32 GPIONumber, uint32 Direction)
{
    /*--------------*/
    /* Check Range. */
    /*--------------*/
    if(GPIONumber <= 9) {		/* XXX THIS PROBABLY SHOULD BE < 8 */
	/*-----------------------*/
	/* Setup gpio direction. */
	/*-----------------------*/
	if (bGalileoGPIOInitialized) {
	    if (Direction == GPIO_OUTPUT)
		GpioConfig.PortADir |= (1 << GPIONumber);
	    else
		GpioConfig.PortADir &= ~(1 << GPIONumber);
	    vGalileoGPIOWrite(GPIO_SWPORTA_DDR, GpioConfig.PortADir);
	}
    }
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
void vGalileoSetGPIOBitLevel(uint32 GPIONumber, uint32 Level)
{
    /*--------------*/
    /* Check Range. */
    /*--------------*/
    if (GPIONumber <= 9) {
	/*--------------------------*/
	/* Set the bit high or low. */
	/*--------------------------*/
	if (bGalileoGPIOInitialized) {
	    /*----------------------*/
	    /* 1 for on, 0 for off. */
	    /*----------------------*/
	    if (Level == HIGH)
		GpioConfig.PortADR |= (1 << GPIONumber);
	    else
		GpioConfig.PortADR &= ~(1 << GPIONumber);
	    vGalileoGPIOWrite(GPIO_SWPORTA_DR, GpioConfig.PortADR);
	}
    }
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void LegacyGpioSetLevel(uint32 RegOffset, uint32 GpioNum,
    uint8 HighLevel)
{
    uint32 RegValue;
    uint32 legacy_gpio_base;
    uint32 GpioNumMask;
    uint8 Bus = LEGACY_GPIO_BUS_NUMBER;
    uint8 Device = LEGACY_GPIO_DEVICE_NUMBER;
    uint8 Func = LEGACY_GPIO_FUNCTION_NUMBER;

    /*----------------------------------*/
    /* Get PCI Configuration IO Address */
    /*----------------------------------*/
    legacy_gpio_base =
	uiGalileoLegacyGPIOPCIRead(IO_PCI_ADDRESS(Bus, Device, Func,
	    R_QNC_LPC_GBA_BASE), B_QNC_LPC_GPA_BASE_MASK);

    /*---------------------------*/
    /* Read register (Port I/O ) */
    /*---------------------------*/
    RegValue = inl(legacy_gpio_base + RegOffset);

    /*-------------------*/
    /* Set Data and mask */
    /*-------------------*/
    GpioNumMask = (1 << GpioNum);
    if (HighLevel)
	RegValue |= (GpioNumMask);
    else
	RegValue &= ~(GpioNumMask);

    /*----------------------------*/
    /* Write the data (Port I/O ) */
    /*----------------------------*/
    outl((legacy_gpio_base + RegOffset), RegValue);
}

/*-------------------------------------------------*/
/* Setup multiplexers to route GPIO_SUS<5> to LED. */
/* Then set the signal level to low (LED off).     */
/*-------------------------------------------------*/
void vGalileoLegacyGPIOInitializationForLED(void)
{
    intmask mask = disable();

    vGalileoRouteLEDPins();

    /*---------------------------*/
    /* Set GPIO_SUS<5> as output */
    /*---------------------------*/
    LegacyGpioSetLevel (
	R_QNC_GPIO_RGIO_RESUME_WELL,				/* 24 */
	GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO,		/* 5 */
	GPIO_OUTPUT);						/* 0 */

    /*------------------------------*/
    /* Set GPIO_SUS<5> level to low */
    /*------------------------------*/
    LegacyGpioSetLevel (
	R_QNC_GPIO_RGLVL_RESUME_WELL,				/* 28 */
	GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO,		/* 5 */
	LOW);							/* 0 */

    restore(mask);
}

/*-------------------------------------*/
/* Set GPIO_SUS<5> to 'Level' (0 or 1) */
/*-------------------------------------*/
void vGalileoBlinkLEDUsingLegacyGPIO(uint32 Level)
{
    intmask mask = disable();

    LegacyGpioSetLevel (
	R_QNC_GPIO_RGLVL_RESUME_WELL,				/* 28 */
        GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO, 		/* 5 == IO13 */
	Level);							/* 0 or 1 */

    resume(mask);
}

/*-----------------------*/
/* I2C support functions */
/*-----------------------*/
/*---------------------------------------------------------------*/
/* Read and return the 64-bit value from the time-stamp counter. */
/*---------------------------------------------------------------*/
static inline uint64 rdtsc(void)
{
    uint32 lo, hi;
    uint64 tsc;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    tsc = hi;
    tsc <<= 32;
    tsc |= lo;
    return tsc;
}


/*-------------------------------------------------------------------------*/
/* Assuming the CPU clock frequency is 400 MHz, delay at least 'DelayTime' */
/* microseconds. This is accomplished by reading the time-stamp counter on */
/* entry and then delaying until at least the specified number of ticks on */
/* the time-stamp counter have occurred. Note that we avoid doing a 64-bit */
/* divide by waiting for the difference between current and initial time-  */
/* stamp counter values is greater than or equal to 400 times the delay    */
/* time. It should be noted that small delay times are less accurately     */
/* produced than longer delay times.                                       */
/*-------------------------------------------------------------------------*/
void vMicroSecondDelay(uint32 DelayTime)
{
    uint64 diff_in_us = 0;		/* counter diff in usec X 400 */
    uint64 cpufreq_in_mhz = 400;
    uint64 tsc_start = rdtsc();
    uint64 tsc_current = tsc_start;

    do {
	diff_in_us = tsc_current - tsc_start;
	tsc_current = rdtsc();
    } while (diff_in_us < cpufreq_in_mhz * DelayTime);
}

/*------------------------------------------*/
/* Delay at least 'DelayTime' milliseconds. */
/*------------------------------------------*/
void vMilliSecondDelay(uint32 DelayTime)
{
    vMicroSecondDelay (DelayTime * 1000);
}

/*--------------------------------------------------*/
/* Return I2C Memory Mapped registers base address. */
/*--------------------------------------------------*/
static uintn GetI2CIoPortBaseAddress(void)
{
    uint8 Bus = IOH_I2C_GPIO_BUS_NUMBER;
    uint8 Device = IOH_I2C_GPIO_DEVICE_NUMBER;
    int8 Func = IOH_I2C_GPIO_FUNCTION_NUMBER;

    uint32 I2C_controller_base = MMIO_PCI_ADDRESS(Bus, Device, Func, 0);
    uintn I2CIoPortBaseAddress = mem_read(I2C_controller_base,
	R_IOH_I2C_MEMBAR, 4);
    return I2CIoPortBaseAddress;
}

/*---------------------------------------------*/
/* Enable the memory-mapped I2C address space. */
/*---------------------------------------------*/
static void EnableI2CMmioSpace(void)
{
    uint8 Bus = IOH_I2C_GPIO_BUS_NUMBER;
    uint8 Device = IOH_I2C_GPIO_DEVICE_NUMBER;
    uint8 Func = IOH_I2C_GPIO_FUNCTION_NUMBER;
    uint32 I2C_controller_base = MMIO_PCI_ADDRESS(Bus, Device, Func, 0);
    uint8 PciCmd = mem_read(I2C_controller_base, PCI_REG_PCICMD, 1);
    mem_write(I2C_controller_base, PCI_REG_PCICMD, 1, (PciCmd | 0x7));
}

/*-----------------------------*/
/* Disable the I2C Controller. */
/*-----------------------------*/
static void DisableI2CController(void)
{
    uintn I2CIoPortBaseAddress;
    uint32 Addr;
    uint32 Data;
    uint8 PollCount = 0;

    /*-----------------------------------------------*/
    /* Get I2C Memory Mapped registers base address. */
    /*-----------------------------------------------*/
    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*-----------------------------------------------------------------*/
    /* Disable the I2C Controller by setting IC_ENABLE.ENABLE to zero. */
    /*-----------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_ENABLE;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data &= ~B_I2C_REG_ENABLE;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*----------------------------------------*/
    /* Read the IC_ENABLE_STATUS.IC_EN Bit to */
    /* check if Controller is disabled.       */
    /*----------------------------------------*/
    Data = 0xFF;
    Addr = I2CIoPortBaseAddress + I2C_REG_ENABLE_STATUS;
    Data = *((volatile uint32 *) (uintn)(Addr)) & I2C_REG_ENABLE_STATUS;
    while (Data != 0) {
	/*------------------------------------------------------------*/
	/* Poll the IC_ENABLE_STATUS.IC_EN Bit to check if Controller */
	/* is disabled, until timeout (TI2C_POLL*MAX_T_POLL_COUNT).   */
	/*------------------------------------------------------------*/
	if (++PollCount >= MAX_T_POLL_COUNT)
	    break;
	vMicroSecondDelay(TI2C_POLL);
	Data = *((volatile uint32 *) (uintn)(Addr));
	Data &= I2C_REG_ENABLE_STATUS;
    }

    /*----------------------------------------------------------------*/
    /* Read IC_CLR_INTR register to automatically clear the combined  */
    /* interrupt, all individual interrupts and the IC_TX_ABRT_SOURCE */
    /* register.                                                      */
    /*----------------------------------------------------------------*/
    if (PollCount < MAX_T_POLL_COUNT) {
	Addr = I2CIoPortBaseAddress + I2C_REG_CLR_INT;
	Data = *((volatile uint32 *) (uintn)(Addr));
    }
}

/*----------------------------*/
/* Enable the I2C Controller. */
/*----------------------------*/
static void EnableI2CController(void)
{
    uintn I2CIoPortBaseAddress;
    uint32 Addr;
    uint32 Data;

    /*-----------------------------------------------*/
    /* Get I2C Memory Mapped registers base address. */
    /*-----------------------------------------------*/
    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*-------------------------------------------------------------*/
    /* Enable the I2C Controller by setting IC_ENABLE.ENABLE to 1. */
    /*-------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_ENABLE;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data |= B_I2C_REG_ENABLE;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*-----------------------------------------------------------------*/
    /* Clear overflow and abort error status bits before transactions. */
    /*-----------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_RX_OVER;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_TX_OVER;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_TX_ABRT;
    Data = *((volatile uint32 *) (uintn)(Addr));
}

/*--------------------------------*/
/* Initialize the I2C Controller. */
/*--------------------------------*/
static uint32 InitializeInternal(I2C_ADDR_MODE AddrMode)
{
    uintn I2CIoPortBaseAddress;
    uintn Addr;
    uint32 Data;
    uint32 Status = 0;

    /*---------------------------------------------*/
    /* Enable access to I2C Controller MMIO space. */
    /*---------------------------------------------*/
    EnableI2CMmioSpace ();

    /*-----------------------------------*/
    /* Disable I2C Controller initially. */
    /*-----------------------------------*/
    DisableI2CController ();

    /*-----------------------------------------------*/
    /* Get I2C Memory Mapped registers base address. */
    /*-----------------------------------------------*/
    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*------------------*/
    /* Clear START_DET. */
    /*------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_START_DET;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data &= ~B_I2C_REG_CLR_START_DET;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*-----------------*/
    /* Clear STOP_DET. */
    /*-----------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_STOP_DET;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data &= ~B_I2C_REG_CLR_STOP_DET;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*------------------------------------------------------------*/
    /* Set addressing mode to user defined (7 or 10 bit) and      */
    /* speed mode to that defined by PCD (standard mode default). */
    /*------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_CON;
    Data = *((volatile uint32 *) (uintn)(Addr));

    /*---------------------.*/
    /* Set Addressing Mode. */
    /*---------------------.*/
    if (AddrMode == EfiI2CSevenBitAddrMode)
	Data &= ~B_I2C_REG_CON_10BITADD_MASTER;
    else
	Data |= B_I2C_REG_CON_10BITADD_MASTER;

    /*-----------------*/
    /* Set Speed Mode. */
    /*-----------------*/
    Data &= ~B_I2C_REG_CON_SPEED;

    /*-----------------------*/
    /* Default to slow mode. */
    /*-----------------------*/
    Data |= BIT1;
    *((volatile uint32 *) (uintn)(Addr)) = Data;
    Data = *((volatile uint32 *) (uintn)(Addr));
    return Status;
}

/*-----------------------------------------------------*/
/* Save GPIO command and base address register (BAR0). */
/*-----------------------------------------------------*/
static void I2cEntry(uint16 *SaveCmdPtr, uint32 *SaveBar0Ptr)
{
    uint8 Bus = IOH_I2C_GPIO_BUS_NUMBER;
    uint8 Device = IOH_I2C_GPIO_DEVICE_NUMBER;
    uint8 Func = IOH_I2C_GPIO_FUNCTION_NUMBER;
    uint32 I2C_controller_base = MMIO_PCI_ADDRESS(Bus, Device, Func, 0);

    I2CGpioBase = mem_read(I2C_controller_base, R_IOH_I2C_MEMBAR, 4);
    *SaveBar0Ptr = I2CGpioBase;
    if (((*SaveBar0Ptr) & B_IOH_I2C_GPIO_MEMBAR_ADDR_MASK) == 0) {
	mem_write(I2C_controller_base, R_IOH_I2C_MEMBAR, 4,
	    IO_PCI_ADDRESS(Bus, Device, Func, 0));
	/*-----------------------------------------------------*/
	/* Also Save Cmd Register, Setup by InitializeInternal */
	/* later during transfers.                             */
	/*-----------------------------------------------------*/
	*SaveCmdPtr = mem_read(I2C_controller_base, PCI_REG_PCICMD, 1);
    }
}

/*-------------------------------*/
/* Restore GPIO command and BAR0 */
/*-------------------------------*/
static void I2cExit(uint16 SaveCmd, uint32 SaveBar0)
{
    if ((SaveBar0 & B_IOH_I2C_GPIO_MEMBAR_ADDR_MASK) == 0) {
	uint8 Bus = IOH_I2C_GPIO_BUS_NUMBER;
	uint8 Device = IOH_I2C_GPIO_DEVICE_NUMBER;
	uint8 Func = IOH_I2C_GPIO_FUNCTION_NUMBER;
	uint32 I2C_controller_base = MMIO_PCI_ADDRESS(Bus, Device, Func, 0);
	mem_write(I2C_controller_base, PCI_REG_PCICMD, 1, SaveCmd);
	mem_write(I2C_controller_base, R_IOH_I2C_MEMBAR, 4, SaveBar0);
    }
}

/*------------------------------------------------------*/
/* Wait for a STOP to be detected on the I2C bus.	*/
/* On success, return 0.				*/
/* On failure (.e.g timeout), return -1 (0xffffffff).	*/
/*------------------------------------------------------*/
static uint32 WaitForStopDet(void)
{
    uintn I2CIoPortBaseAddress;
    uint32 Addr;
    uint32 Data;
    uint32 PollCount = 0;
    uint32 Status = 0;

    /*-----------------------------------------------*/
    /* Get I2C Memory Mapped registers base address. */
    /*-----------------------------------------------*/
    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*-----------------------*/
    /* Wait for STOP Detect. */
    /*-----------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_RAW_INTR_STAT;
    do {
	Data = *((volatile uint32 *) (uintn)(Addr));
	if ((Data & I2C_REG_RAW_INTR_STAT_TX_ABRT) != 0) {
	    Status = -1;
	    break;
	}
	if ((Data & I2C_REG_RAW_INTR_STAT_TX_OVER) != 0) {
	    Status = -1;
	    break;
	}
	if ((Data & I2C_REG_RAW_INTR_STAT_RX_OVER) != 0) {
	    Status = -1;
	    break;
	}
	if ((Data & I2C_REG_RAW_INTR_STAT_STOP_DET) != 0) {
	    Status = 0;
	    break;
	}
	vMicroSecondDelay(TI2C_POLL);
	PollCount++;
	if (PollCount >= MAX_STOP_DET_POLL_COUNT) {
	    Status = -1;
	    break;
	}
    } while (TRUE);

    return Status;
}

/*-----------------------------------------------------------------------*/
/* Send 'Length' bytes from WriteBuffer to the I2C device at I2CAddress. */
/* Return -1 on failures.						 */
/*-----------------------------------------------------------------------*/
uint32 WriteMultipleByte(uintn I2CAddress, uint8 *WriteBuffer,
    uintn Length)
{
    uintn I2CIoPortBaseAddress;
    uintn Index;
    uintn Addr;
    uint32 Data;
    uint32 Status = 0;

    if (Length > I2C_FIFO_SIZE)
	return -1;  /* we do not handle xfers > fifo size. */

    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*-------------------------------------------------*/
    /* Write to the IC_TAR register the address of the */
    /* slave device to be addressed.                   */
    /*-------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_TAR;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data &= ~B_I2C_REG_TAR;
    Data |= I2CAddress;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*----------------------------*/
    /* Enable the I2C Controller. */
    /*----------------------------*/
    EnableI2CController ();

    /*--------------------------------------------------------------------*/
    /* Write the data and transfer direction to the IC_DATA_CMD register. */
    /* Also specify that transfer should be terminated by STOP condition. */
    /*--------------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_DATA_CMD;
    for (Index = 0; Index < Length; Index++) {
	Data = *((volatile uint32 *) (uintn)(Addr));
	Data &= 0xFFFFFF00;
	Data |= (uint8)WriteBuffer[Index];
	Data &= ~B_I2C_REG_DATA_CMD_RW;
	if (Index == (Length-1))
	    Data |= B_I2C_REG_DATA_CMD_STOP;
	*((volatile uint32 *) (uintn)(Addr)) = Data;
    }

    /*-------------------------------*/
    /* Wait for transfer completion. */
    /*-------------------------------*/
    Status = WaitForStopDet ();

    /*---------------------------------*/
    /* Ensure I2C Controller disabled. */
    /*---------------------------------*/
    DisableI2CController ();

    return Status;
}

/*-----------------------------------------------------------------------*/
/* Invoke the internal initialization of the I2C controller as needed.   */
/* THen send Send 'Length' bytes from 'Buffer' to the I2C device at      */
/* SlaveAddress. Use the specified address mode (7 or 10 bits).		 */
/*-----------------------------------------------------------------------*/
static void I2CWriteMultipleBytes(I2C_DEVICE_ADDRESS  SlaveAddress,
    I2C_ADDR_MODE AddrMode, uintn *Length, void *Buffer)
{
    uintn I2CAddress;
    uint16 SaveCmd;
    uint32 SaveBar0;

    if (Buffer != NULL && Length != NULL) {
	SaveCmd = 0;
	SaveBar0 = 0;
	I2cEntry (&SaveCmd, &SaveBar0);
	if (InitializeInternal(AddrMode) == 0) {
	    I2CAddress = SlaveAddress.I2CDeviceAddress;
	    WriteMultipleByte(I2CAddress, Buffer, (*Length));
	}
	I2cExit (SaveCmd, SaveBar0);
    }
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
uint32 ReadMultipleByte(uintn I2CAddress, uint8 *Buffer,
	uintn WriteLength, uintn ReadLength)
{
    uintn I2CIoPortBaseAddress;
    uintn Index;
    uintn Addr;
    uint32 Data;
    uint8 PollCount;
    uint32 Status;

    if (WriteLength > I2C_FIFO_SIZE || ReadLength > I2C_FIFO_SIZE)
	return -1;  /* We do not handle transfers > fifo size. */

    I2CIoPortBaseAddress = GetI2CIoPortBaseAddress ();

    /*-------------------------------------------------*/
    /* Write to the IC_TAR register the address of the */
    /* slave device to be addressed/                   */
    /*-------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_TAR;
    Data = *((volatile uint32 *) (uintn)(Addr));
    Data &= ~B_I2C_REG_TAR;
    Data |= I2CAddress;
    *((volatile uint32 *) (uintn)(Addr)) = Data;

    /*----------------------------*/
    /* Enable the I2C Controller. */
    /*----------------------------*/
    EnableI2CController ();

    /*-------------------------------------------------------------*/
    /* Write the data (sub-addresses) to the IC_DATA_CMD register. */
    /*-------------------------------------------------------------*/
    Addr = I2CIoPortBaseAddress + I2C_REG_DATA_CMD;
    for (Index = 0; Index < WriteLength; Index++) {
	Data = *((volatile uint32 *) (uintn)(Addr));
	Data &= 0xFFFFFF00;
	Data |= (uint8)Buffer[Index];
	Data &= ~B_I2C_REG_DATA_CMD_RW;
	*((volatile uint32 *) (uintn)(Addr)) = Data;
    }

    /*----------------------------------------------------*/
    /* Issue Read Transfers for each byte. Restart issued */
    /* when write/read bit changed).                      */
    /*----------------------------------------------------*/
    for (Index = 0; Index < ReadLength; Index++) {
	Data = *((volatile uint32 *) (uintn)(Addr));
	Data |= B_I2C_REG_DATA_CMD_RW;

	/*--------------------------------------*/
	/* Issue a STOP for last read transfer. */
	/*--------------------------------------*/
	if (Index == (ReadLength-1))
	    Data |= B_I2C_REG_DATA_CMD_STOP;
	*((volatile uint32 *) (uintn)(Addr)) = Data;
    }

    /*--------------------------*/
    /* Wait for STOP condition. */
    /*--------------------------*/
    Status = WaitForStopDet();
    if (Status != 0) {
	/*---------------------------------------------*/
	/* Poll Receive FIFO Buffer Level register     */
	/*  until valid (upto MAX_T_POLL_COUNT times). */
	/*---------------------------------------------*/
	Data = 0;
	PollCount = 0;
	Addr = I2CIoPortBaseAddress + I2C_REG_RXFLR;
	Data = *((volatile uint32 *) (uintn)(Addr));
	while ((Data != ReadLength) && (PollCount < MAX_T_POLL_COUNT)) {
	    vMicroSecondDelay(TI2C_POLL);
	    PollCount++;
	    Data = *((volatile uint32 *) (uintn)(Addr));
	}

	Addr = I2CIoPortBaseAddress + I2C_REG_RAW_INTR_STAT;
	Data = *((volatile uint32 *) (uintn)(Addr));

	/*--------------------------------------------------*/
	/* If no timeout or device error then read rx data. */
	/*--------------------------------------------------*/
	if (PollCount == MAX_T_POLL_COUNT) {
	    Status = -1;
	} else if ((Data & I2C_REG_RAW_INTR_STAT_RX_OVER) != 0) {
	    Status = -1;
	} else {
	    /*------------------------------------------------*/
	    /* Clear RX underflow before reading IC_DATA_CMD. */
	    /*------------------------------------------------*/
	    Addr = I2CIoPortBaseAddress + I2C_REG_CLR_RX_UNDER;
	    Data = *((volatile uint32 *) (uintn)(Addr));

	    /*------------*/
	    /* Read data. */
	    /*------------*/
	    Addr = I2CIoPortBaseAddress + I2C_REG_DATA_CMD;
	    for (Index = 0; Index < ReadLength; Index++) {
		Data = *((volatile uint32 *) (uintn)(Addr));
		Data &= 0x000000FF;
		*(Buffer+Index) = (uint8)Data;
	    }
	    Addr = I2CIoPortBaseAddress + I2C_REG_RAW_INTR_STAT;
	    Data = *((volatile uint32 *) (uintn)(Addr));
	    Data &= I2C_REG_RAW_INTR_STAT_RX_UNDER;
	    if (Data != 0) {
		Status = -1;
	    } else {
		Status = 0;
	    }
	}
    }

    /*---------------------------------*/
    /* Ensure I2C Controller disabled. */
    /*---------------------------------*/
    DisableI2CController ();

    return Status;
}

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void I2CReadMultipleBytes(I2C_DEVICE_ADDRESS SlaveAddress,
    I2C_ADDR_MODE AddrMode, uintn *WriteLength, uintn *ReadLength,
    void *Buffer )
{
    uintn I2CAddress;
    uint16 SaveCmd;
    uint32 SaveBar0;
    uint32 Status;

    if (Buffer != NULL && WriteLength != NULL && ReadLength != NULL) {
	SaveCmd = 0;
	SaveBar0 = 0;
	I2cEntry (&SaveCmd, &SaveBar0);
	if (InitializeInternal(AddrMode) == 0) {
	    I2CAddress = SlaveAddress.I2CDeviceAddress;
	    Status = ReadMultipleByte(I2CAddress, Buffer, (*WriteLength),
		(*ReadLength));
	}
	I2cExit (SaveCmd, SaveBar0);
    }
}

/*-------------------------------------------------------------------*/
/* Pcal9535 chips used on Galileo Gen 2 boards (see FAB-H schematic) */
/* The functions are inappropriately named '9555' instead of 9535.   */
/*-------------------------------------------------------------------*/
static void Pcal9555SetPortRegBit(
    uint32 Pcal9555SlaveAddr,
    uint32 GpioNum,
    uint8 RegBase,
    uint8 LogicOne)
{
    uintn ReadLength;
    uintn WriteLength;
    uint8 Data[2];
    uint8 *RegValuePtr;
    uint8 GpioNumMask;
    uint8 SubAddr;
    I2C_DEVICE_ADDRESS I2cDeviceAddr;
    I2C_ADDR_MODE I2cAddrMode;

    /*---------------------------*/
    /* Set I2C address and mode. */
    /*---------------------------*/
    I2cDeviceAddr.I2CDeviceAddress = (uintn)Pcal9555SlaveAddr;
    I2cAddrMode = EfiI2CSevenBitAddrMode;

    /*-----------------------------------*/
    /* Set I2C subaddress and GPIO mask. */
    /*-----------------------------------*/
    if (GpioNum < 8) {
	SubAddr = RegBase;
	GpioNumMask = (uintn) (1 << GpioNum);
    } else {
	SubAddr = RegBase + 1;
	GpioNumMask = (uintn) (1 << (GpioNum - 8));
    }

    /*--------------------------------------------------------*/
    /* Output port value always at 2nd byte in Data variable. */
    /*--------------------------------------------------------*/
    RegValuePtr = &Data[1];

    /*-----------------------------------------------*/
    /* On read entry - sub address at 2nd byte;      */
    /* on read exit - output port value in 2nd byte. */
    /*-----------------------------------------------*/
    Data[1] = SubAddr;
    WriteLength = 1;
    ReadLength = 1;
    I2CReadMultipleBytes(
	I2cDeviceAddr,
	I2cAddrMode,
	&WriteLength,
	&ReadLength,
	&Data[1]);

    /*------------------------------------------*/
    /* Adjust output port bit using mask value. */
    /*------------------------------------------*/
    if (LogicOne)
	*RegValuePtr = *RegValuePtr | GpioNumMask;
    else
	*RegValuePtr = *RegValuePtr & ~(GpioNumMask);

    /*--------------------------------------------------------------*/
    /* Update register. Sub address at 1st byte, value at 2nd byte. */
    /*--------------------------------------------------------------*/
    WriteLength = 2;
    Data[0] = SubAddr;
    I2CWriteMultipleBytes(
	I2cDeviceAddr,
	I2cAddrMode,
	&WriteLength,
	Data);
}

/*--------------------------------------------------------------*/
/* Configure the internal pullup/pulldown for a specified pin.	*/
/* There are three possible settings for 'enable':		*/
/*	0 = no internal pullup/pulldown enabled			*/
/*	1 = internal pulldown enabled				*/
/*	2 = internal pullup enabled				*/
/*--------------------------------------------------------------*/
static void PlatformPcal9555GpioPullup(
    uint32 Pcal9555SlaveAddr,			/* I2C address of a PCAL 9535 */
    uint32 GpioNum,				/* GPIO number (0..15) */
    uint32 enable)				/* 0=disable, 1=enable */
{
    if (enable > 2)				/* ignore bad values */
	return;

    if (enable == 0) {
	Pcal9555SetPortRegBit(			/* disable pull up / down */
	    Pcal9555SlaveAddr,
	    GpioNum,
	    PCAL9555_REG_PULL_EN_PORT0,
	    0 );
	return;
    }

    Pcal9555SetPortRegBit(			/* enable pull up / down */
	Pcal9555SlaveAddr,
	GpioNum,
	PCAL9555_REG_PULL_EN_PORT0,
	1 );

    Pcal9555SetPortRegBit(			/* set pull up / down */
	Pcal9555SlaveAddr,
	GpioNum,
	PCAL9555_REG_PULL_PORT0,
	enable-1 );				/* 0 = pulldown, 1 = pullup */

}

/*-----------------------------------------------------------------------*/
/* Configure the specified pin on the specified EXP chip as input if the */
/* CfgAsInput argument is TRUE (1). Otherwise configure it as output.    */
/*-----------------------------------------------------------------------*/
static void PlatformPcal9555GpioSetDir(
    uint32 Pcal9555SlaveAddr,			/* I2C address of chip */
    uint32 GpioNum,				/* which GPIO */
    uint32 CfgAsInput)				/* TRUE=input, FALSE=output */
{
    Pcal9555SetPortRegBit(
	Pcal9555SlaveAddr,
	GpioNum,
	PCAL9555_REG_CFG_PORT0,
	CfgAsInput);
}

/*-----------------------------------------------------------*/
/* Set the level (0=low, 1=high) of the specified pin on the */
/* specified EXP child.					     */
/*-----------------------------------------------------------*/
static void PlatformPcal9555GpioSetLevel(
    uint32 Pcal9555SlaveAddr,			/* I2C address of chip */
    uint32 GpioNum,				/* which GPIO */
    uint32 HighLevel )				/* 1 = high, 0 = low */
{
    Pcal9555SetPortRegBit(
	Pcal9555SlaveAddr,
	GpioNum,
	PCAL9555_REG_OUT_PORT0,
	HighLevel );
}

/*----------------------------*/
/* GPIO pin routing functions */
/*----------------------------*/

/*-----------------------------*/
/* XXX Needs documentation XXX */
/*-----------------------------*/
static void vGalileoRouteLEDPins(void)
{
    /*----------------------------------------------------------------*/
    /* For GpioNums below values 0 to 7 are for Port0 IE. P0-0 - P0-7 */
    /* and values 8 to 15 are for Port1 IE. P1-0 - P1-7. Disable      */
    /* pull-ups / pull downs on EXP0 pin for LVL_B_PU7 signal.        */
    /*----------------------------------------------------------------*/
    PlatformPcal9555GpioPullup (
	GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  /* (I2C = 25) IO Expander 0. */
	15,                                   /* P1-7. */
	0
    );

    /*---------------------------------*/
    /* Make LVL_B_OE7_N an output pin. */
    /*---------------------------------*/
    PlatformPcal9555GpioSetDir (
	GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  /* IO Expander 0. */
	14,                                   /* P1-6. */
	FALSE
    );

    /*----------------------------------*/
    /* Set level of LVL_B_OE7_N to low. */
    /*----------------------------------*/
    PlatformPcal9555GpioSetLevel (
	GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,
	14,
	FALSE
    );

    /*------------------------------*/
    /* Make MUX8_SEL an output pin. */
    /*------------------------------*/
    PlatformPcal9555GpioSetDir (
	GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  /* IO Expander 1. */
	14,                                   /* P1-6. */
	FALSE
    );

    /*-----------------------------------------------------------*/
    /* Set level of MUX8_SEL to low to route GPIO_SUS<5> to LED. */
    /*-----------------------------------------------------------*/
    PlatformPcal9555GpioSetLevel (
	GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  /* IO Expander 1. */
	14,                                   /* P1-6. */
	FALSE
    );
}
