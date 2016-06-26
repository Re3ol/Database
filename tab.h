#pragma once
#include "DBManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;

struct rawdataTab {
	string* _attrName;       //name of attribute.
	string* _attrRstr;		 //restriction of atrribute.
	int _attrCount;			 //Count of attribute.

	int _PK_pos;				 //Primary Key position.

	//Constructor.
	rawdataTab() {
		_attrName = NULL;
		_attrRstr = NULL;
		_attrCount = 0;

		_PK_pos = 0;
	}		
	//Constructor with parameters
	rawdataTab(string* attrName, int attrCount) {
		_attrName = new string[attrCount];
		_attrRstr = new string[attrCount];
		_attrCount = attrCount;
		_PK_pos = 0;

		for (int i = 0; i < attrCount; i++)
		{
			_attrName[i] = attrName[i];
			_attrRstr[i] = "NULL";
		}
	}

};

class tab {
private:
	string _tabPath;          //the postion where the table is stored.
	string _rawtabPath;		  //the postion where the raw data is stored.
	string _tabName;          //the name of the table.

	line* _line;			  //the instance lines of the table.
	int _lineCount;			  //the count of all lines.
	
	rawdataTab _rawData;      //the data which stores the raw information such as attributes or restrictions.

	bTree PK_index;	          //the default Btree index in the Primary Key attribute.
public: 
	//Constructor.
	tab();					 
	
	// Create a table using parsed data from a SQL.
	bool Create(string tabName, string* attrName, string* attrRstr, int attrCount);
 
	//Insert a table using parsed data from a SQL.
	bool Insert(rawdataTab attr, string* values);

	//Search a key value using parsed data from a SQL.
	string* SearchNShow(string right, string middle, string left ,int &count/*, SEARCH_MODE SearchMode*/);

	//Delete a key value using parsed data from a SQL.
	bool Delete(string right, string middle, string left/*, SEARCH_MODE SearchMode*/);

	//Display the table.
	void Display();

private:
	//set the [_rawData] using the raw table in [_rawtabPath].
	void setRawdata(string* attrName, string* attrRstr, int attrCount, int PK_pos);  

	// Insert a piece of line into the current table. 
	bool Insert(line &inLine);

	//Search a key value using parsed data from a SQL.
	string* Search(string right, string middle, string left, int &count/*, SEARCH_MODE SearchMode*/);

	// Delete a piece of line from the current table. 
	bool Delete(line &inLine);

	//If the table has a inserted line, Initialize the class.
	void Initialize();
};