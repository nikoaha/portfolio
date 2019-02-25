/*
 * ITM_output.cpp
 *
 *  Created on: 25 Jan 2019
 *      Author: Laptop
 */

#include "ITM_output.h"
#include "ITM_write.h"
using namespace std;


ITM_output::ITM_output() {
	// TODO Auto-generated constructor stub
    ITM_init();

}

ITM_output::~ITM_output() {
	// TODO Auto-generated destructor stub
}


void ITM_output::print(std::string output){
	ITM_write(output.c_str());
}

void ITM_output::print(char output[]){
	ITM_write(output);
}


void ITM_output::print(int number){
	string output = to_string(number);
	ITM_write(output.c_str());
}

