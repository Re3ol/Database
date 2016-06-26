#pragma once
#include "line.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
struct sItem
{
	string data;
	int seq;
	
	//Constructor
	sItem() {
		data = "";
		seq = 0;
	}
	//Constructor
	sItem(string _data, int _seq) {
		data = _data;
		seq = _seq;
	}

	bool operator==(const sItem &right) const;  // Overload [==] funtion.
	bool operator<(const sItem &right) const;   // Overload [<] funtion.
	bool operator>(const sItem &right) const;   // Overload [>] funtion.
	sItem& operator=(sItem &right);             // Overload [=] funtion for another sItem.
	sItem& operator=(const string& s);			// Overload [=] funtion for a constant string.

	friend ostream& operator<<(ostream& os, sItem& s);  //overload [<<] function for print.
};


class index{
	friend class line;

public:
	char *read(string filename);                           //Read file named 'filename'.
	
	line *parseToLine(char* data, int &lineCount);		   //Parse the data into single lines.
	
	sItem *getColumn(line *data, int lineCount, int pos);  //get the data in the [pos]th column.
	
	void convert(sItem in, string &key, string &value);	   //convert sitem to key and value.
};

