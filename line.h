#pragma once
#include "DBManager.h"
#include <iostream>
#include <string>

using namespace std;

class line {
public:
	int dataLength;					// The length of line.
	int seq;						// The sequence of the line in [data] table. 
	char* raw;						// The raw data of the exact line.
	string* item;					// The single item of the line.

public:	
	//Constructor
	line() {
		dataLength = 0;
		seq = 0;
		
		raw = NULL;
		item = NULL;
	}
	
	//Constructor
	line(int _dataLength, int _seq, char* _raw) {
		dataLength = _dataLength;
		seq = _seq;
		raw = _raw;
	}

	void show();						//Show the info of the line.	
	void split();						//Split the raw data into single items.
	void setAttr(int _dataLength, int _seq, char* _raw);	//Set the attribute of line.

	//bool operator==(const line &right) const;  // Overload [==] funtion.
	line& operator=(const line &right);        // Overload [=] funtion for const line.
	line& operator=(line &right);              // Overload [=] funtion for variable line.
};