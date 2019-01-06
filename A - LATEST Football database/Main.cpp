// COMMENT:
// This is a football match database project made with C++.
// The implementation technique here is a vector, which contains the class MatchInfo, which contains a single match data.
// Every operation is executed by using this vector. It is found at line 69.
// No major bugs found and is enough publishable.
//----------------------------------------------------------------------------

/*

	Final project C++
	Period 2 - 2018
	Niko Haapalainen
	1706789

	Football match results service v1.0

*/

#include "MatchInfo.h"
#include "Tools.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3
#define BUTTON_4 4
#define BUTTON_5 5
#define BUTTON_6 6
#define BUTTON_7 7
#define BUTTON_8 8
#define BUTTON_9 9
#define COLOR_WHITE 15
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define VECTOR_SIZE_EMPTY 0
#define MAX_INT_WIDTH 12 // the column char size 10 + the gap 2 
#define MAX_STR_WIDTH 22 // the column char size 20 + the gap 2
#define MAX_STR_SIZE 20 // maximum number of characters in a string
#define MAX_MATCHES 100

int mainMenu();
void addMatch(vector<MatchInfo>&);
void viewData(vector<MatchInfo>);
void saveData(vector<MatchInfo>);
void readData(vector<MatchInfo>&);
void clearData(vector<MatchInfo>&);
int findVectorSize(vector<MatchInfo>&);
void editData(vector<MatchInfo>&);
void findTeamMatches(vector<MatchInfo>);
void findTeamMatchLocations(vector<MatchInfo>);

int main() {
	mainMenu();
	return 0;
}

// Main menu selection screen.
int mainMenu() {
	vector<MatchInfo> matchList;
	Tools tools;
	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	int selection = 0;

	while (1) {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "FOOTBALL RESULT SERVICE v1.0" << endl;
		cout << "By Niko Haapalainen" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "  -- MAIN MENU --" << endl << endl;
		cout << "Enter corresponding index number to act..." << endl << endl;

		cout << "1) Add new match" << endl;
		cout << "2) View match database" << endl;
		cout << "3) Save records to disk" << endl;
		cout << "4) Read records from disk" << endl;
		cout << "5) Clear records" << endl;
		cout << "6) Make a correction to an existing match" << endl;
		cout << "7) Print a report of a team's played matches" << endl;
		cout << "8) Print a report of matches played by location" << endl;
		cout << "9) Quit" << endl << endl;

		cin >> selection;
		tools.clearCinBuffer();

		system("cls");

		if (selection == BUTTON_1)
		{
			addMatch(matchList);
		}
		else if (selection == BUTTON_2)
		{
			viewData(matchList);
		}
		else if (selection == BUTTON_3)
		{
			saveData(matchList);
		}
		else if (selection == BUTTON_4)
		{
			readData(matchList);
		}
		else if (selection == BUTTON_5)
		{
			clearData(matchList);
		}
		else if (selection == BUTTON_6)
		{
			editData(matchList);
		}
		else if (selection == BUTTON_7)
		{
			findTeamMatches(matchList);
		}
		else if (selection == BUTTON_8)
		{
			findTeamMatchLocations(matchList);
		}
		else if (selection == BUTTON_9)
		{
			break;
		}

		system("cls");
	}

	return 0;
}

// Add a match to the database
void addMatch(vector<MatchInfo>& addMatch) {

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	string teamName;
	string location;
	string teamScore;
	string weekNumber;
	bool rangeSwitch = false;
	Tools tools;

	// The object handled for this function
	MatchInfo match;

	// check if user is attempting to add more than 100 matches.
	if (addMatch.size() < MAX_MATCHES) {

		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "ADD MATCH" << endl << "The maximum is 100 matches." << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "Enter data..." << endl << endl;

		// Home team input
		while (1) {
			cout << "Home team name: ";
			getline(cin, teamName);

			if (tools.isInputValid(teamName) == true) {
				match.setHomeTeamName(teamName);
				break;
			}
		}

		// Opposing team input
		while (1) {
			cout << "Opposing team name: ";
			getline(cin, teamName);

			if (tools.isInputValid(teamName) == true) {
				match.setOpposingTeamName(teamName);
				break;
			}
		}

		// Location input
		while (1) {
			cout << "Match location (city, arena...): ";
			getline(cin, location);
			match.setMatchLocation(location);

			if (tools.isInputValid(location) == true) {
				match.setMatchLocation(location);
				break;
			}
		}

		// Home team score input
		while (1) {
			cout << "Home team's score (0 - 200): ";

			// To test if the input is an integer AND a valid integer
			getline(cin, teamScore);
			if (tools.isIntInputValid(teamScore, rangeSwitch) == true) {
				match.setHomeTeamScore(stoi(teamScore));
				break;
			}
		}

		// Opposing team score input
		while (1) {

			cout << "Opposing team's score (0 - 200): ";

			getline(cin, teamScore);
			if (tools.isIntInputValid(teamScore, rangeSwitch) == true) {
				match.setOpposingTeamScore(stoi(teamScore));
				break;
			}
		}

		// Week number input
		rangeSwitch = true;
		while (1) {
			cout << "Week number of match (1 - 52): ";

			getline(cin, weekNumber);
			if (tools.isIntInputValid(weekNumber, rangeSwitch) == true) {
				match.setWeekNumOfMatch(stoi(weekNumber));
				break;
			}
		}

		// The 'match' object of this function is moved
		// to the parameter's vector's end here.
		addMatch.push_back(match);

		SetConsoleTextAttribute(colorer, COLOR_GREEN);
		cout << endl << "Match data added." << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

	}

	// IF database full...
	else {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "ADD MATCH" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "The database is full!" << endl;
		cout << "No more than 100 matches!" << endl << endl;
	}
	
	system("pause");
	system("cls");
}

// View match database
void viewData(vector<MatchInfo> viewMatchInfo) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(colorer, COLOR_RED);
	cout << "VIEW MATCH DATABASE" << endl << endl;
	SetConsoleTextAttribute(colorer, COLOR_WHITE);

	// Print each element's contents out of the vector
	for (unsigned int i = 0; i < viewMatchInfo.size(); i++) {
		cout << "Match #" << i + 1 << endl;
		cout << "Home team name:\t\t" << viewMatchInfo[i].getHomeTeamName() << endl;
		cout << "Opposing team name:\t" << viewMatchInfo[i].getOpposingTeamName() << endl;
		cout << "Match location:\t\t" << viewMatchInfo[i].getMatchLocation() << endl;
		cout << "Home team score:\t" << viewMatchInfo[i].getHomeTeamScore() << endl;
		cout << "Opposing team score:\t" << viewMatchInfo[i].getOpposingTeamScore() << endl;
		cout << "Week number:\t\t" << viewMatchInfo[i].getWeekNumOfMatch() << endl << endl;
	}

	// If there are contents in the vector, print the end tag
	if (viewMatchInfo.size() > VECTOR_SIZE_EMPTY) {
		cout << "  -- End of data --" << endl << endl;
	}
	else { // If no contents
		cout << "No data found." << endl << endl;
	}

	system("pause");
}

// Save records to disk
void saveData(vector<MatchInfo> matchInfoSaver) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	Tools tools;

	int selection = 0;

	SetConsoleTextAttribute(colorer, COLOR_RED);
	cout << "SAVE RECORDS TO DISK" << endl << endl;
	SetConsoleTextAttribute(colorer, COLOR_WHITE);

	if (matchInfoSaver.size() == VECTOR_SIZE_EMPTY) {
		SetConsoleTextAttribute(colorer, COLOR_YELLOW);
		cout << "NOTICE: No data in the current database." << endl;
		cout << "        Saving the empty file would clear the data in the current memory file." << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);
	}

	cout << "The current memory file data will be overwritten!" << endl;
	cout << "Proceed?" << endl << endl;
	cout << "1) Cancel" << endl;
	cout << "2) Overwrite and save" << endl << endl;

	while (1) {
		cin >> selection;
		tools.clearCinBuffer();

		if (selection == BUTTON_1) {
			break;
		}
		else if (selection == BUTTON_2) {
			ofstream memoryFileOutput("memory.txt");
			memoryFileOutput << matchInfoSaver; // <- Call the save file operator
			memoryFileOutput.close();

			SetConsoleTextAttribute(colorer, COLOR_GREEN);
			cout << endl << "File saved successfully!" << endl << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);

			system("pause");
			break;
		}
		else {
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "->ERROR: Invalid input. Enter 1 or 2 to act." << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
		}
	}
}

void readData(vector<MatchInfo>& matchInfoLoader) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	Tools tools;

	int selection = 0;
	ifstream memoryFileInput("memory.txt");

	SetConsoleTextAttribute(colorer, COLOR_RED);
	cout << "READ RECORDS FROM DISK" << endl << endl;
	SetConsoleTextAttribute(colorer, COLOR_WHITE);

	// Warning message if the current memory file is empty.
	if (memoryFileInput.is_open()) {
		// IF findVectorSize() functions returned size is 0, show NOTICE msg.
		if (tools.findVectorSize(matchInfoLoader) == VECTOR_SIZE_EMPTY) {
			SetConsoleTextAttribute(colorer, COLOR_YELLOW);
			cout << "NOTICE: No data in the memory file." << endl;
			cout << "        Loading the empty file would clear the current data in the database." << endl << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
		}

		cout << "If loaded, the current data will be overwritten!" << endl;
		cout << "Proceed?" << endl << endl;
		cout << "1) Cancel" << endl;
		cout << "2) Overwrite and load" << endl << endl;

		while (1) {
			cin >> selection;

			if (selection == BUTTON_1) {
				break;
			}
			else if (selection == BUTTON_2) {
				matchInfoLoader.clear();
				tools.findVectorSize(matchInfoLoader);

				// Call the load file operator
				memoryFileInput >> matchInfoLoader;
				memoryFileInput.close();

				SetConsoleTextAttribute(colorer, COLOR_GREEN);
				cout << endl << "File loaded." << endl << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);

				system("pause");
				break;
			}
			else {
				SetConsoleTextAttribute(colorer, COLOR_RED);
				cout << "->ERROR: Invalid input. Enter 1 or 2 to act." << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);
			}

			tools.clearCinBuffer();
		}
	}
	// If no memory file found.
	else {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "->ERROR: Memory file not found." << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);
		cout << "         To bypass the error, save the current record to create a file." << endl << endl;

		system("pause");
	}
}

void clearData(vector<MatchInfo>& matchInfoClear) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);

	int selection = 0;
	int subSelection = 0;
	bool mainWhileLoopOn = false;
	Tools tools;

	while (1) {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "CLEAR RECORDS" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "1) Clear all current record data" << endl;
		cout << "2) Clear all records in the save file" << endl;
		cout << "3) Cancel" << endl << endl;

		while (1)
		{
			cin >> selection;

			// OPTION 1: CLEAR CURRENT DATA
			if (selection == BUTTON_1)
			{
				cout << endl << "Are you sure to clear current data?" << endl << endl;

				cout << "1) Cancel" << endl;
				cout << "2) Proceed" << endl << endl;

				while (1) {

					cin >> subSelection;

					if (subSelection == BUTTON_1) {
						break;
					}
					else if (subSelection == BUTTON_2) {
						matchInfoClear.clear();
						SetConsoleTextAttribute(colorer, COLOR_GREEN);
						cout << endl << "Current data cleared successfully." << endl << endl;
						SetConsoleTextAttribute(colorer, COLOR_WHITE);

						system("pause");
						break;
					}
					else {
						SetConsoleTextAttribute(colorer, COLOR_RED);
						cout << "->ERROR: Invalid input. Enter 1 or 2 to act." << endl;
						SetConsoleTextAttribute(colorer, COLOR_WHITE);
					}

					tools.clearCinBuffer();
				}

				break; // to break the parent while
			}

			// OPTION 2: CLEAR SAVE FILE
			else if (selection == BUTTON_2)
			{
				cout << endl << "Are you sure to clear records in the save file?" << endl << endl;

				cout << "1) Cancel" << endl;
				cout << "2) Proceed" << endl << endl;

				while (1) {

					cin >> subSelection;

					if (subSelection == BUTTON_1) {
						break;
					}
					else if (subSelection == BUTTON_2) {
						ofstream wipeData;
						wipeData.open("memory.txt", ofstream::out | ofstream::trunc);
						wipeData.close();

						SetConsoleTextAttribute(colorer, COLOR_GREEN);
						cout << endl << "Save file cleared successfully." << endl << endl;
						SetConsoleTextAttribute(colorer, COLOR_WHITE);

						system("pause");

						break;
					}
					else {
						SetConsoleTextAttribute(colorer, COLOR_RED);
						cout << "->ERROR: Invalid input. Enter 1 or 2 to act." << endl;
						SetConsoleTextAttribute(colorer, COLOR_WHITE);
					}

					tools.clearCinBuffer();
				}

				break; // break this while
			}

			// TO CANCEL
			else if (selection == BUTTON_3)
			{
				mainWhileLoopOn = true;
				break;
			}

			// IF FALSE INPUT
			else
			{
				SetConsoleTextAttribute(colorer, COLOR_RED);
				cout << "->ERROR: Invalid input. Enter 1, 2 or 3 to act." << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);
			}
			tools.clearCinBuffer();
		}

		if (mainWhileLoopOn == true) {
			mainWhileLoopOn = false;
			break;
		}

		tools.clearCinBuffer();
		system("cls");
	}
}

// Make a correction
void editData(vector<MatchInfo>& matchInfoEdit)
{
	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);

	unsigned int selection = 0;
	unsigned int i = 0;
	bool mainBreak = false;
	bool finalBreak = false;
	bool rangeSwitch = false;
	string newInt = "";
	string input = "";
	Tools tools;

	if (matchInfoEdit.size() != 0)
	{
		while (1)
		{
			// Screen 1
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "MAKE A CORRECTION TO AN EXISTING MATCH" << endl << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);

			cout << "Choose a match to edit:" << endl;
			for (unsigned int j = 0; j < matchInfoEdit.size(); j++)
			{
				cout << j + 1 << ") " << matchInfoEdit[j].getHomeTeamName() << " vs. "
					<< matchInfoEdit[j].getOpposingTeamName() << endl;
			}
			cout << endl << "Enter 0 to cancel." << endl << endl;

			while (1) {
				cin >> selection;
				tools.clearCinBuffer();

				if (selection != BUTTON_0 && matchInfoEdit.size() > selection - 1) {
					selection--;
					i = selection;
					break;
				}
				else if (selection == BUTTON_0) {
					mainBreak = true;
					break;
				}
				else {
					SetConsoleTextAttribute(colorer, COLOR_RED);
					cout << "->ERROR: Invalid input. Enter any of the list's numbers." << endl;
					SetConsoleTextAttribute(colorer, COLOR_WHITE);
				}
			}

			if (mainBreak == true) {
				mainBreak = false;
				break;
			}

			system("cls");

			while (1) {
				// Screen 2
				SetConsoleTextAttribute(colorer, COLOR_RED);
				cout << "MAKE A CORRECTION TO AN EXISTING MATCH" << endl << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);

				cout << "Now editing match: " << matchInfoEdit[i].getHomeTeamName() << " vs. "
					<< matchInfoEdit[i].getOpposingTeamName() << endl << endl;

				cout << "Choose the element to edit:" << endl;
				cout << "1) Home team name" << endl;
				cout << "2) Opposing team name" << endl;
				cout << "3) Match location" << endl;
				cout << "4) Home team score" << endl;
				cout << "5) Opposing team score" << endl;
				cout << "6) Week number of match" << endl << endl;

				cout << "Enter 0 to cancel." << endl << endl;

				while (1) {
					cin >> selection;
					tools.clearCinBuffer();

					// The valid choices here are BUTTON_1, BUTTON_2, BUTTON_3, ... , BUTTON_6
					if (selection >= BUTTON_1 && BUTTON_6 >= selection) {
						break;
					}
					else if (selection == BUTTON_0) {
						mainBreak = true;
						break;
					}
					else {
						SetConsoleTextAttribute(colorer, COLOR_RED);
						cout << "->ERROR: Invalid input. Enter any of the list's numbers." << endl;
						SetConsoleTextAttribute(colorer, COLOR_WHITE);
					}
				}

				if (mainBreak == true) {
					mainBreak = false;
					finalBreak = true;
					break;
				}

				system("cls");

				// Screen 3
				SetConsoleTextAttribute(colorer, COLOR_RED);
				cout << "MAKE A CORRECTION TO AN EXISTING MATCH" << endl << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);

				cout << "Now editing match: " << matchInfoEdit[i].getHomeTeamName() << " vs. "
					<< matchInfoEdit[i].getOpposingTeamName() << endl << endl;

				if (selection == BUTTON_1)
				{
					cout << "Enter new data for home team name..." << endl << endl;
					while (1) {
						getline(cin, input);
						if (tools.isInputValid(input) == true) {
							matchInfoEdit[i].setHomeTeamName(input);
							break;
						}
					}
				}
				else if (selection == BUTTON_2)
				{
					cout << "Enter new data for opposing team name..." << endl << endl;
					while (1) {
						getline(cin, input);
						if (tools.isInputValid(input) == true) {
							matchInfoEdit[i].setOpposingTeamName(input);
							break;
						}
					}
				}
				else if (selection == BUTTON_3)
				{
					cout << "Enter new data for match location..." << endl << endl;
					while (1) {
						getline(cin, input);
						if (tools.isInputValid(input) == true) {
							matchInfoEdit[i].setMatchLocation(input);
							break;
						}
					}
				}
				else if (selection == BUTTON_4)
				{
					cout << "Enter new score for home team score..." << endl << endl;
					while (1) {
						getline(cin, newInt);
						if (tools.isIntInputValid(newInt, rangeSwitch) == true) {
							matchInfoEdit[i].setHomeTeamScore(stoi(newInt));
							break;
						}
					}
				}
				else if (selection == BUTTON_5)
				{
					cout << "Enter new score for opposing team score..." << endl << endl;
					while (1) {
						getline(cin, newInt);
						if (tools.isIntInputValid(newInt, rangeSwitch) == true) {
							matchInfoEdit[i].setOpposingTeamScore(stoi(newInt));
							break;
						}
					}
				}
				else if (selection == BUTTON_6)
				{
					cout << "Enter new number for week number..." << endl << endl;
					rangeSwitch = true;
					while (1) {
						getline(cin, newInt);
						if (tools.isIntInputValid(newInt, rangeSwitch) == true) {
							matchInfoEdit[i].setWeekNumOfMatch(stoi(newInt));
							break;
						}
					}
				}
				else
				{
					SetConsoleTextAttribute(colorer, COLOR_RED);
					cout << "->ERROR: Invalid input. Enter corresponding match numbers printed above." << endl;
					SetConsoleTextAttribute(colorer, COLOR_WHITE);
				}

				SetConsoleTextAttribute(colorer, COLOR_GREEN);
				cout << endl << "New data added successfully." << endl << endl;
				SetConsoleTextAttribute(colorer, COLOR_WHITE);

				system("pause");
				system("cls");
				break;
			}

			if (finalBreak == true) {
				finalBreak = false;
				break;
			}

			//break;
		}
		system("cls");
	}

	// If no matches in the database
	else
	{
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "MAKE A CORRECTION TO AN EXISTING MATCH" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "No existing matches found." << endl << endl;
		system("pause");
	}

	system("cls");
}

// Find a team's played matches
void findTeamMatches(vector<MatchInfo> findMatch) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	int selection = 0;
	int count = 0;
	bool breaker = false;
	unsigned int i = 0;
	vector<string> teamSortList;
	string last = "";
	string tempString = "";
	Tools tools;

	if (findMatch.size() != 0) {
		while (1) {
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "PRINT A REPORT OF A TEAM'S PLAYED MATCHES" << endl << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);

			cout << "Teams found in the database:" << endl;

			// First find all the teams and push them to the teamSortList vector...
			for (unsigned int j = 0; j < findMatch.size(); j++) {
				teamSortList.push_back(findMatch[j].getHomeTeamName());
				teamSortList.push_back(findMatch[j].getOpposingTeamName());
			}

			// sort them and finally remove the duplicates with 'unique' func template
			sort(teamSortList.begin(), teamSortList.end());
			teamSortList.erase(unique(teamSortList.begin(), teamSortList.end()), teamSortList.end());

			for (unsigned int j = 0; j < teamSortList.size(); j++) {
				cout << ++count << ") " << teamSortList[j] << endl;
			}

			count = 0;
			cout << endl << "Enter 0 to cancel." << endl << endl;

			while (1) {
				cin >> selection;
				i = selection - 1;

				tools.clearCinBuffer();

				// selection-1 = vector index number
				if (selection > BUTTON_0 && teamSortList.size() > selection - 1) {
					break;
				}
				else if (selection == BUTTON_0) {
					breaker = true;
					break;
				}
				else {
					SetConsoleTextAttribute(colorer, COLOR_RED);
					cout << "->ERROR: Invalid input. Enter corresponding index numbers printed above." << endl << endl;
					SetConsoleTextAttribute(colorer, COLOR_WHITE);
				}
			}

			// to exit the function
			if (breaker == true) {
				breaker = false;
				break;
			}

			system("cls");

			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "MATCHES OF TEAM: ";
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
			cout << teamSortList[i] << endl << endl;

			cout << setw(MAX_STR_WIDTH) << left << "Home team" << setw(MAX_STR_WIDTH) << left << "Opposing team";
			cout << setw(MAX_INT_WIDTH) << left << "Home score" << setw(MAX_INT_WIDTH) << left << "Opp. score";
			cout << setw(MAX_STR_WIDTH) << left << "Location" << setw(MAX_INT_WIDTH) << left << "Week number" << endl << endl;

			for (unsigned int j = 0; j < findMatch.size(); j++)
			{
				// Print, if the chosen team's name matches the home team's name AND 
				// is not in the opposing team name i.e. is not having a match against itself.
				if (teamSortList[i] == findMatch[j].getHomeTeamName() && teamSortList[i] != findMatch[j].getOpposingTeamName()) {
					cout << setw(MAX_STR_WIDTH) << left << teamSortList[i]; // home team
					cout << setw(MAX_STR_WIDTH) << left << findMatch[j].getOpposingTeamName(); // opp team
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getHomeTeamScore();
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getOpposingTeamScore();
					cout << setw(MAX_STR_WIDTH) << left << findMatch[j].getMatchLocation();
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getWeekNumOfMatch() << endl;
				}
				// If the chosen team is not the home team in a match, then try to find as an opposing team.
				else if (teamSortList[i] == findMatch[j].getOpposingTeamName() && teamSortList[i] != findMatch[j].getHomeTeamName()) {
					cout << setw(MAX_STR_WIDTH) << left << findMatch[j].getHomeTeamName(); // home team
					cout << setw(MAX_STR_WIDTH) << left << teamSortList[i]; // opp team
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getHomeTeamScore();
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getOpposingTeamScore();
					cout << setw(MAX_STR_WIDTH) << left << findMatch[j].getMatchLocation();
					cout << setw(MAX_INT_WIDTH) << left << findMatch[j].getWeekNumOfMatch() << endl;
				}
			}

			cout << endl;
			system("pause");
			system("cls");
		}
	}
	else {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "PRINT A REPORT OF A TEAM'S PLAYED MATCHES" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "No existing matches found." << endl << endl;
		system("pause");
	}

	system("cls");
}

// Find matches by location
void findTeamMatchLocations(vector<MatchInfo> findMatchLoc) {

	system("cls");

	HANDLE colorer = GetStdHandle(STD_OUTPUT_HANDLE);
	int selection = 0;
	bool breaker = false;
	int count = 0;
	unsigned int i = 0;
	vector<string> locationList;
	Tools tools;

	if (findMatchLoc.size() != 0) {
		while (1) {
			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "PRINT A REPORT OF MATCHES PLAYED BY LOCATION" << endl << endl;
			SetConsoleTextAttribute(colorer, COLOR_WHITE);

			cout << "Found locations in the database:" << endl;

			// First find all the teams and push them to the teamSortList vector...
			for (unsigned int j = 0; j < findMatchLoc.size(); j++) {
				locationList.push_back(findMatchLoc[j].getMatchLocation());
			}

			// sort them and finally remove the duplicates with 'unique' func template
			sort(locationList.begin(), locationList.end());
			locationList.erase(unique(locationList.begin(), locationList.end()), locationList.end());

			for (unsigned int j = 0; j < locationList.size(); j++) {
				cout << ++count << ") " << locationList[j] << endl;
			}

			count = 0;
			cout << endl << "Enter 0 to cancel." << endl << endl;

			while (1) {
				cin >> selection;
				i = selection - 1;

				tools.clearCinBuffer();

				// selection-1 = vector index number
				if (selection > BUTTON_0 && locationList.size() > selection - 1) {
					break;
				}
				else if (selection == BUTTON_0) {
					breaker = true;
					break;
				}
				else {
					SetConsoleTextAttribute(colorer, COLOR_RED);
					cout << "->ERROR: Invalid input. Enter corresponding index numbers printed above." << endl << endl;
					SetConsoleTextAttribute(colorer, COLOR_WHITE);
				}
			}

			// to exit the function
			if (breaker == true) {
				breaker = false;
				break;
			}

			system("cls");

			SetConsoleTextAttribute(colorer, COLOR_RED);
			cout << "MATCHES PLAYED IN LOCATION: ";
			SetConsoleTextAttribute(colorer, COLOR_WHITE);
			cout << locationList[i] << endl << endl;

			cout << setw(MAX_STR_WIDTH) << left << "Home team" << setw(MAX_STR_WIDTH) << left << "Opposing team";
			cout << setw(MAX_INT_WIDTH) << left << "Home score" << setw(MAX_INT_WIDTH) << left << "Opp. score";
			cout << setw(MAX_STR_WIDTH) << left << "Location" << setw(MAX_INT_WIDTH) << left << "Week number" << endl << endl;

			for (unsigned int j = 0; j < findMatchLoc.size(); j++)
			{
				// Print, if the chosen location's name matches to the running locations name.
				if (locationList[i] == findMatchLoc[j].getMatchLocation()) {
					cout << setw(MAX_STR_WIDTH) << left << findMatchLoc[j].getHomeTeamName();
					cout << setw(MAX_STR_WIDTH) << left << findMatchLoc[j].getOpposingTeamName();
					cout << setw(MAX_INT_WIDTH) << left << findMatchLoc[j].getHomeTeamScore();
					cout << setw(MAX_INT_WIDTH) << left << findMatchLoc[j].getOpposingTeamScore();
					cout << setw(MAX_STR_WIDTH) << left << findMatchLoc[j].getMatchLocation();
					cout << setw(MAX_INT_WIDTH) << left << findMatchLoc[j].getWeekNumOfMatch() << endl;
				}
			}

			cout << endl;
			system("pause");
			system("cls");
		}
		system("cls");
	}
	// If no matches in current database.
	else {
		SetConsoleTextAttribute(colorer, COLOR_RED);
		cout << "PRINT A REPORT OF MATCHES PLAYED BY LOCATION" << endl << endl;
		SetConsoleTextAttribute(colorer, COLOR_WHITE);

		cout << "No existing matches found." << endl << endl;
		system("pause");
	}

}
