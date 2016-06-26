#include "line.h"

using namespace std;

//
//Show the info of the line.
//
void line::show()
{
	cout << "sequence number: " << seq << endl
		<< "data length: " << dataLength << endl
		<< "raw data: " << raw << endl;
}

//
//Split the raw data into single items.
//
void line::split() {
	int i = 0, j = 0;

	int _orCount = 0;
	string str = raw;

	//Count the items.
	for (i = 0; i<str.length(); i++)
	{
		if (str[i] == '|')
			_orCount++;
	}
	
	int *_orPos = new int[_orCount]();
	
	//Record all the '|' postion of the raw data.
	for(i=0;i<str.length();i++)
	{
		if (str[i] == '|')	
			_orPos[j++] = i;
	}

	item = new string[_orCount - 1];

	//Assign the items.
	for (i = 0; i < _orCount - 1; i++)
		//cout<<str.substr(_orPos[i] + 1, _orPos[i + 1] - _orPos[i] - 1)<<endl;
		item[i] = str.substr(_orPos[i] + 1, _orPos[i + 1] - _orPos[i] - 1);

	//cout << "Split over!" << endl;
	
	delete[] _orPos;
	_orPos = NULL;
}

//
//Set the attribute of line.
//
void line::setAttr(int _dataLength, int _seq, char* _raw) {
	raw = new char;
	item = new string;

	dataLength = _dataLength;
	seq = _seq;
	
	for (int i = 0; i < _dataLength; i++) {
		raw[i] = _raw[i];
			
	}
}

//
// Overload [=] funtion for const line.
//
line& line::operator=(const line &right) {
	dataLength = right.dataLength;
	seq = right.seq;

	raw = right.raw;
	item = right.item;

	return *this;
}

//
// Overload [=] funtion for variable line.
//
line& line::operator=(line &right) {
	dataLength = right.dataLength;
	seq = right.seq;

	raw = right.raw;
	//item = right.item; 

	return *this;
}