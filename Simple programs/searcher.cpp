// Niko Haapalainen 1706789
// C++ programming in Smart Systems, period 2, 2018
// Exercise 5

/**
This program looks up to the weather.xml file
and prints the wanted inner text from inside the tags.

The wanted text for this program from the .xml file are...

                ...
	        <suggested_pickup_period>60</suggested_pickup_period>
  this ->   <location>New York/John F. Kennedy Intl Airport, NY</location>
	        <station_id>KJFK</station_id>
  this ->   <temp_c>25</temp_c>
                ...
**/



#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string find_field(string page, const char text[50])
{
	string string1 = page;
	string str = text;
	char leftover = '>';

	// cut </tag> and after
	size_t found1 = string1.rfind(str);
	if (found1 != std::string::npos) {
		string1.erase(found1-2, string1.length());
	}

	// cut <tag> and before
	size_t found2 = string1.rfind(str);
	if (found2 != std::string::npos) {
		string1.erase(0, found2 + str.length() + 1);
	}

	// if leftovers occur, cut them too
	size_t found3 = string1.rfind(leftover);
	if (found3 != std::string::npos) {
		string1.erase(0, found3+1);
	}

	return string1;
}

int main() {
	string page, line, location, temperature;
	ifstream inputFile("weather.xml");
	
	while (getline (inputFile, line)) {
 	    page.append(line);
 	    line.erase();
	}

	location = find_field(page, "location");
	temperature = find_field(page, "temp_c");

	cout << "Location: " << location << endl;
	cout << "Temperature: " << temperature << endl; 

	system("pause");
}





/**
    OUTPUT:
    Location: New York/John F. Kennedy Intl Airport, NY
    Temperature: 25
**/
