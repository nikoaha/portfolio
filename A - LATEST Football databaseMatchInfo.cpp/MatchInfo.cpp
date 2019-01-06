#include "MatchInfo.h"
#include "Tools.h"
#include <iostream>
#define NUM_OF_IGNORE_ENDLINE 6

void MatchInfo::setHomeTeamName(string x) {
	homeTeamName = x;
}

string MatchInfo::getHomeTeamName() {
	return homeTeamName;
}

void MatchInfo::setOpposingTeamName(string x) {
	opposingTeamName = x;
}

string MatchInfo::getOpposingTeamName() {
	return opposingTeamName;
}

void MatchInfo::setHomeTeamScore(int x) {
	homeTeamScore = x;
}

int MatchInfo::getHomeTeamScore() {
	return homeTeamScore;
}

void MatchInfo::setOpposingTeamScore(int x) {
	opposingTeamScore = x;
}

int MatchInfo::getOpposingTeamScore() {
	return opposingTeamScore;
}

void MatchInfo::setMatchLocation(string x) {
	matchLocation = x;
}

string MatchInfo::getMatchLocation() {
	return matchLocation;
}

void MatchInfo::setWeekNumOfMatch(int x) {
	weekNumOfMatch = x;
}

int MatchInfo::getWeekNumOfMatch() {
	return weekNumOfMatch;
}

// to read from file
istream& operator >> (istream& in, vector<MatchInfo>& match) {
	for (unsigned int i = 0; i < match.size(); i++) {
		getline(in, match[i].homeTeamName);
		getline(in, match[i].opposingTeamName);
		getline(in, match[i].matchLocation);
		in >> match[i].homeTeamScore;
		in >> match[i].opposingTeamScore;
		in >> match[i].weekNumOfMatch;
		in.ignore(NUM_OF_IGNORE_ENDLINE, '\n'); // To ignore the endlines
	}
	return in;
}

// to save to a file
ostream& operator << (ostream& out, const vector<MatchInfo> match) {
	for (unsigned int i = 0; i < match.size(); i++) {
		out << match[i].homeTeamName << endl;
		out << match[i].opposingTeamName << endl;
		out << match[i].matchLocation << endl;
		out << match[i].homeTeamScore << endl;
		out << match[i].opposingTeamScore << endl;
		out << match[i].weekNumOfMatch << endl;
	}
	return out;
}
