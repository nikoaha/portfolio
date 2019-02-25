/*
 ===============================================================================
 Name        : main.c
 Author      : Niko H
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <cr_section_macros.h>

#include <atomic>
static volatile std::atomic_int counter;
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Handle interrupt from SysTick timer
 * @return Nothing
 */

void SysTick_Handler(void) {
	if (counter > 0)
		counter--;
}
#ifdef __cplusplus
}
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#define I2C_MODE    (0)
#define I2C_CLK_DIVIDER         (40)
#define I2C_BITRATE         (55000)
static I2CM_XFER_T i2cmXferRec;

static void Init(void) {
#if defined(BOARD_KEIL_MCB1500)||defined(BOARD_NXP_LPCXPRESSO_1549)
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
#else
#error "No I2C Pin Muxing defined for this example"
#endif
}

/* Setup I2C handle and parameters */
static void Setup() {
	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	 do this */
	Chip_I2C_Init(LPC_I2C0);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(LPC_I2C0, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);

	/* Enable Master Mode */
	Chip_I2CM_Enable(LPC_I2C0);
}

static void Read(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize,
		uint8_t *rxBuffPtr, uint16_t rxSize) {
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}

#include "ITM_write.h"
#include "ITM_output.h"

int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	ITM_init();
	Init();
	Setup();
	Board_Init();

	// Set the LED to the state of "On"
#endif
#endif

	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 9,
			(IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 9);

	uint8_t temperature = 0;
	uint8_t status = 0;

	uint8_t cmdTemp = 0x00;
	uint8_t cmdStatus = 0x01;
	ITM_output out;

	/* Read LM75 temperature sensor */

	Board_LED_Set(2, false);

	while (1) {
		if (Chip_GPIO_GetPinState(LPC_GPIO, 1, 9)) {
			Read(0x4A, &cmdStatus, 1, &status, 1);
			if ((status & 0x40) != 0) {
				Read(0x4A, &cmdTemp, 1, &temperature, 1);
				Board_LED_Set(2, true);
				if ((temperature & 0x80) != 0) {
					int8_t negTemp = temperature;
					out.print(negTemp);
					out.print(" degrees\n");
				} else {
					out.print(temperature);
					out.print(" degrees\n");
				}
			}
			else {
				out.print("status is not ready\n");
				Board_LED_Set(2, false);
			}
		}
	}
	return 0;
}
