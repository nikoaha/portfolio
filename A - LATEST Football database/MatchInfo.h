#pragma once
// header

#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <string>
#include <vector>
using namespace std;

class MatchInfo {
public:
	void setHomeTeamName(string x);
	string getHomeTeamName();

	void setOpposingTeamName(string x);
	string getOpposingTeamName();

	void setHomeTeamScore(int x);
	int getHomeTeamScore();

	void setOpposingTeamScore(int x);
	int getOpposingTeamScore();

	void setMatchLocation(string x);
	string getMatchLocation();

	void setWeekNumOfMatch(int x);
	int getWeekNumOfMatch();

	friend istream& operator >> (istream& in, vector<MatchInfo>& match);
	friend ostream& operator << (ostream& out, const vector<MatchInfo> match);

private:
	string homeTeamName;
	string opposingTeamName;
	int homeTeamScore;
	int opposingTeamScore;
	string matchLocation;
	int weekNumOfMatch;
};

#endif 
