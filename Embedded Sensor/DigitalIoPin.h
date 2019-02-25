/*
 * DigitalIoPin.h
 *
 *  Created on: 21.1.2019
 *      Author: Niko
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_


class DigitalIoPin {
public:
	DigitalIoPin(int port, int pin, bool input = true, bool pullup = true, bool invert = false);
	virtual ~DigitalIoPin();
	bool read();
	void write(bool value);
private:
	int port;
	int pin;
	// add these as needed
};


#endif /* DIGITALIOPIN_H_ */
