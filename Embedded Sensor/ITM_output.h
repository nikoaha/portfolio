/*
 * ITM_output.h
 *
 *  Created on: 25 Jan 2019
 *      Author: Laptop
 */

#ifndef ITM_OUTPUT_H_
#define ITM_OUTPUT_H_
#include <string>

class ITM_output {
public:
	ITM_output();
	virtual ~ITM_output();
	void print(std::string output);
	void print(char output[]);
	void print(int number);
};

#endif /* ITM_OUTPUT_H_ */
