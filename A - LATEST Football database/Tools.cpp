#include "MatchInfo.h"
#include "Tools.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <fstream>
#include <sstream>
using namespace std;

#define MAX_CHAR_IGNORE 10000
#define MAX_ELEMENTS_IN_A_MATCH_OBJECT 6
#define VECTOR_SIZE_EMPTY 0
#define EMPTY_STRING ""
#define MAX_STR_SIZE 20
#define COLOR_WHITE 15
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define MIN_GOALS 0
#define MAX_GOALS 200
#define MIN_WEEK_NUM 1
#define MAX_WEEK_NUM 52

// cin buffer needs to be cleared here occasionally to avoid input skipping.
void Tools::clearCinBuffer() {
	cin.clear();
	cin.ignore(MAX_CHAR_IGNORE, '\n');
}

// A tool for resizing the vector properly for the function readData().
int Tools::findVectorSize(vector<MatchInfo>& matchInfoReSize) {
	char ch = ' ';
	int numOfElements = 0;
	int temp = 0;
	ifstream memoryFile("memory.txt");

	while (!memoryFile.eof())
	{
		memoryFile.get(ch);
		{
			if (ch == '\n')
			{
				temp++;

				if (temp == MAX_ELEMENTS_IN_A_MATCH_OBJECT) {
					numOfElements++;
					temp = 0;
				}
			}
		}
	}
	memoryFile.close();

	// If no contents inside the memory file, skip the resizing.
	if (numOfElements != VECTOR_SIZE_EMPTY) {
		matchInfoReSize.resize(numOfElements);
	}

	// Return the size for the vector for the size checker (For the NOTICE:...).
	return numOfElements;
}

// A tool for finding valid string input
bool Tools::isInputValid(string inputStr) {
	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);

	if (inputStr == EMPTY_STRING) {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "->ERROR: The team name must contain something!" << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);
	}
	else if (inputStr.size() > MAX_STR_SIZE) {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "->ERROR: The team name must be max 20 characters!" << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);
	}
	else { // if valid, return true.
		return true;
	}
}

// To check if assumed integer input is valid
bool Tools::isIntInputValid(string inputStr, bool rangeSwitch) {
	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	bool isInteger = false;
	int thisInt = 0;

	// First compare string input (which contains an integer character)
	// to stringstream -> if integer found in the string, continue.
	stringstream testIfInteger(inputStr);
	if (testIfInteger >> thisInt)
	{
		if (testIfInteger.eof())
		{
			isInteger = true;
		}
	}
	else {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "->ERROR: Invalid integer!" << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);
	}

	// If integer was found, check if it is in range
	// range for goals
	if (isInteger == true && rangeSwitch == false) {
		if (thisInt >= MIN_GOALS && MAX_GOALS >= thisInt)
		{
			return true;
		}
		else
		{
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "->NOTICE: Valid score range is 0 - 200." << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
		}
	}

	// range for weeks
	if (isInteger == true && rangeSwitch == true) {
		if (thisInt >= MIN_WEEK_NUM && MAX_WEEK_NUM >= thisInt)
		{
			return true;
		}
		else
		{
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "->NOTICE: Valid score range is 1 - 52." << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
		}
	}
}
