#include "index.h"

using namespace std;

//
//Read the file named "filename", return the pointer to the raw data.
//
char *index::read(string filename) {
	//raw data of file named "filename"
	char* raw = NULL;
	int dataLength = 0;
	ifstream _INF;
	
	//Read the file named "filename" and do not create a new file if NOT exist.
	_INF.open(filename, ios::_Nocreate);
	
	//Return if the file is not open.
	if (_INF.fail())
	{
		cout << "Reading file failed " << endl;
		return raw;
	}

	//Get the length of _INF stream
	_INF.seekg(0, _INF.end);
	dataLength = _INF.tellg();
	raw = new char[dataLength];

	//Read data of _INF stream
	_INF.seekg(0, _INF.beg);
	_INF.read(raw,dataLength);	

	_INF.close();
	cout << "Reading success!" << endl;
	return raw;
}

//
//Parse the data read into exact numbers of lines, return the pointer to the parsed data as line.
//
line *index::parseToLine(char* data, int &lineCount) {
	int i = 0, j = 0, k = 0;
	int sharpCount = 0;
	
	//Count the sharps.
	i = 0;
	while (data[i] != '\0')
	{
		if (data[i] == '#')
			sharpCount++;
		i++;
	}

	i = 0; j = 0;
	int *sharp = new int[sharpCount]();
	
	//Record the place of '#'.
	while (data[i] != '\0' )
	{
		if (data[i] == '#')
			//cout << i << " ";
			sharp[j++] = i;
		
		i++;
	}	

	//lines of raw data after parsed 
	line *lap = new line[sharpCount - 1]();
	
	//The process of Parsing
	//Each line is headed with "#[sequence]" and datas are divided by "|".
	j = 0;
	while (j < sharpCount - 1)
	{
		k = 0;
		lap[j].dataLength = sharp[j + 1] - sharp[j] - 2;
		lap[j].raw = new char[lap[j].dataLength + 1];

		for (i = sharp[j] + 2; i < sharp[j + 1]; i++)
			//cout << data[i]
			lap[j].raw[k++] = data[i];
		
		lap[j].raw[k] = '\0';
		lap[j].seq = j;
		//cout << lap[j].data << endl;
		j++;
	}

		/*while (data[i] != '\n')
		{ //Case '#'
			if (data[i] == '#') {
				i++;
				while (data[i] != '|')
					_seq[k++] = data[i++];

				//Assign the sequence to current line.
				lap[j].seq = atoi(_seq - '0');

				delete _seq;
				_seq = new char;
				k = 0;
				i++;
			}
			//Case raw data
			else {
				while (data[i] != '|')
					_item[k++] = data[i++];

				//Assign the sequence to current line.
				lap[j].item = _item;

				//Go to the next line
				delete _item;
				_item = new char;
				k = 0;
				i++;
				j++;
			}
		}

		delete _seq;
		delete _item;*/
	
	lineCount = sharpCount - 1;
	cout << "Parsing success!"<<endl;
	
	delete[] sharp;
	sharp = NULL;

	return lap;
}

//
//get the data in the [pos]th column, return the pointer to the column as sItems.
//
sItem* index::getColumn(line *data, int lineCount, int pos) {
	int i = 0;
	
	//The result is saved in the struct sItem called res.
	sItem* res = new sItem[lineCount]();
	
	//Assign the items to the struct sItem called res.
	for (i = 0; i < lineCount; i++) {
		 res[i].data = data[i].item[pos - 1];
		 res[i].seq = i;
	}

	return res;
}

//
//Convert sItem to key and value.
//
void index::convert(sItem in, string &key, string &value) {
	stringstream ss;

	key = in.data;
	ss << in.seq;
	ss >> value;
}

//
// Overload [==] funtion.
//
bool sItem::operator==(const sItem &right) const {
	if (right.data == data)
		return true;

	else
		return false;
}

//
// Overload [<] funtion.
//
bool sItem::operator<(const sItem &right) const {
	if (data < right.data)
		return true;

	else
		return false;
}

//
// Overload [>] funtion.
//
bool sItem::operator>(const sItem &right) const {
	if (data > right.data)
		return true;

	else
		return false;
}

//
// Overload [=] funtion.
//
sItem &sItem::operator=(sItem &right) {
	string tmp;
	tmp = right.data;
	data = tmp;

	seq = right.seq;

	return *this;
}

//
// Overload [=] funtion for a constant string.
//
sItem &sItem::operator=(const string& s) {
	data = s;

	return *this;
}

//
//overload [<<] function for print.
//
ostream& operator<<(ostream& os, sItem& s) {
	os << s.data;
	return os;
}