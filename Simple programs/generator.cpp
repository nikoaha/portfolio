// Niko Haapalainen 1706789
// C++ programming in Smart Systems, period 2, 2018
// Exercise 1

/**
The program asks the user to print the wanted amount 
of randomly generated numbers and eventually calculates
the mean of the generated numbers.
**/

/** START **/

#include <iostream>
#include <cstdlib>

using namespace std;

int mean(int *array, int n)
{
	int sum = 0;
	int thisMean = 0;

	// Finds the sum portion for the calculation
	for (int i = 0;i<n;i++)
	{
		sum = sum + array[i];
	}

	// formula is mean = (sum / quantity)
	thisMean = sum / n;

	return thisMean;
}

int main() {
	int size = 0;
	int thisMean = 0;

	// User input for the size
	cout << "Enter a fine amount of numbers [1-100]: ";
	cin >> size;
	
	// Allocates a block of memory of a 100 slots
	int *p = new int[100];

	// Fills the block of memory with random numbers ranging 0-99
	for (int j=0 ; j<size ; j++) {
		p[j] = rand() % 100; 
	}

	// ...and prints them
	for (int j = 0; j < size; j++) {
		cout << p[j] << "\n";
	}

	// Calls the 'mean' function 
	// Calculates and prints the result
	thisMean = mean(p, size);

	// Prints the result
	cout << "The mean of these numbers: " << thisMean;

	// Frees the data of the allocated memory space
	delete[] p;

	// To prevent the console from closing 
	system("pause");
	cout << endl;

	return 0;
}


/** END **/




/**
    OUTPUT:
    Enter a fine amount of numbers [1-100]: 5
    41
    67
    34
    0
    69
    The mean of these numbers: 42
**/
