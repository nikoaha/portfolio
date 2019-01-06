#pragma once
// header

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>
using namespace std;

class Tools {
public:
	void clearCinBuffer();
	int findVectorSize(vector<MatchInfo>& x);
	bool isInputValid(string x);
	bool isIntInputValid(string x, bool y);
};

#endif 
