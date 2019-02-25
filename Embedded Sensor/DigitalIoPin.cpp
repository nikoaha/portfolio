/*
 * DigitalIoPin.cpp
 *
 *  Created on: 21.1.2019
 *      Author: Niko
 */


#include "board.h"
#include "chip.h"
#include "DigitalIoPin.h"
#include "ITM_write.h"

DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert) {
	// INPUT WITH PULLUP
	if ((input == true) && (pullup == true) && (invert == true)) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN ));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	//INPUT WITH PULLUP - INVERT THE BUTTON READING
	else if ((input == true) && (pullup == false) && (invert == false)) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	//INPUT WITH PULLUP - INVERT THE BUTTON READING
	else if ((input == true) && (pullup == true) && (invert == false)) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	//INPUT WITH PULLUP - INVERT THE BUTTON READING
	else if ((input == true) && (pullup == false) && (invert == true)) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN ));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	// OUTPUT
	else if (input == false) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}


	DigitalIoPin::port = port;
	DigitalIoPin::pin = pin;

}

DigitalIoPin::~DigitalIoPin() {

}

bool DigitalIoPin::read() {
	return Chip_GPIO_GetPinState(LPC_GPIO, port , pin);
}

void DigitalIoPin::write(bool value) {
	Chip_GPIO_SetPinState(LPC_GPIO, port , pin, value);
}


