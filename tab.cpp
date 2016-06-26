#include "tab.h"


//
//Constructor.
//
tab::tab() {
	_tabPath  = "";
	_rawtabPath = "";
	_tabName = "";

	_line = NULL;
	_lineCount = 0;

	setRawdata(NULL, NULL, 0 ,PK_DEFAULT_POS);

	PK_index = bTree(3);
}

//
//Create a table using parsed data from a SQL.
//Parameter:
//tabName: the name of the table.
//attrName: the string of attribute names.
//attrRstr: the string of attribute restictions.
//attrCount:the count of attribute.
//
bool tab::Create(string tabName, string* attrName, string* attrRstr, int attrCount) {
	ifstream _INF;
	ofstream _1OUTF;	
	ofstream _2OUTF;
	int PK_pos;
	bool isTabExist = false;

	string tabPath = TAB_PATH + tabName + ".tab";
	string rawtabPath = RAWTAB_PATH + tabName + ".rawtab";

	//Judge if the table is or not exsited.
	_INF.open(tabPath, ios::_Nocreate);
	if (!_INF.fail())
	{
		cout << "Table exsits already!\n";
		isTabExist = true;
	}
	_INF.close();

	//The table is not exsited
	if (!isTabExist) {
		_1OUTF.open(tabPath,ios::app);				//create the table file named tabName.
		_1OUTF << "#";
		_2OUTF.open(rawtabPath);			//create the raw table file named rawtabName.
		cout << "Create success!\n";
	}

	if (_1OUTF.fail() || _2OUTF.fail())
	{
		cout << "Create table failed " << endl;
		return false;
	}

	//Write the attribute sequence, name, restriction into the rawtab file. 
	for (int i = 0; i < attrCount; i++) {
		if(!isTabExist)
			_2OUTF << "#|" << i << "|" << attrName[i] << "|" << attrRstr[i] << "|";
		if (attrRstr[i] == "PK")
			PK_pos = i;
	}

	//Assign the private member.
	_tabName = tabName;
	_tabPath = tabPath;
	_rawtabPath = rawtabPath;

	setRawdata(attrName, attrRstr, attrCount,PK_pos);
	Initialize();

	_1OUTF.close();
	_2OUTF.close();

	return true;
}

//
//Insert a table using parsed data from a SQL.
//Parameter:
//attr:the list of attribute name which is going to be inserted.
//values:the string of values which is going to be inserter.
//
bool tab::Insert(rawdataTab attr, string* values) {
	int PK = _rawData._PK_pos;
	int rela_PK = 0;
	int tmpAttrPos[MAX_ATTR_BUFFER];
	bool PK_EXSIT = false;			

	//If the number of attribute is more than rawdata attribute count, return false.
	if (attr._attrCount > _rawData._attrCount)
	{
		cout << "The number of attribute is more than expected!\n";
		return false;
	}

	//Loop to record the inserted attribute position.
	for (int i = 0; i < attr._attrCount; i++) {
		for (int j = 0; j < _rawData._attrCount; j++) {
			if (attr._attrName[i] == _rawData._attrName[j]) {
				tmpAttrPos[i] = j;
			
				if (j == PK) {
					rela_PK = i;
					PK_EXSIT = true;
				}
			}
		}
	}
	
	//If the Primary Key of inserted is not provided, return false.	
	if (!PK_EXSIT) {
		cout << "Primary Key in not privided!\n";
		return false;
	}

	//If the Primary Key is duplicated, return false;
	if (_line != NULL) {	
		for (int i = 0; i < _lineCount; i++) {		
			if (_line[i].item[PK] == values[rela_PK])
			{
				cout << "Primary Key duplicated error!\n";
				return false;
			}
		}
	}

	//If the table is not create at first, return false.
	ofstream _OUTF;
	_OUTF.open(_tabPath, ios::_Nocreate);
	if (_OUTF.fail()) {
		cout << "Please create table first!\n";
		return false;
	}

	_OUTF.close();
	//new line insertion process.
	stringstream ss;
	string tmp;
	ss << _lineCount;
	ss >> tmp;
	
	int j = 0;
	string _raw = "|" + tmp + "|";
	
	//Write the data into _raw for insertion.
	for (int i = 0; i < _rawData._attrCount; i++) {
		//The [i]th attribute position has values.
		if (tmpAttrPos[j] == i) 
			_raw = _raw + values[j++] + "|";

		//The [i]th attribute position is NULL
		else
			_raw += "NULL|";
	}
	_raw += "#";

	//Insert new line into member [_line].
	line tmpLine;
	tmpLine.setAttr(_raw.length() + 1, _lineCount, (char*)_raw.data());
	Insert(tmpLine);
	
	//Append new line into file [_tabPath.tab].
	ofstream tmpStream;
	tmpStream.open(_tabPath, ios::app);
	tmpStream << _raw;
	tmpStream.close();

	cout << "Insert success!\n";
	return true;
}

//
//Search for an item in an B-tree and show the result.
//Parameters:
//right: the right value of the expression.
//middle:the middle value of the expression.
//left:the left value of the expression.
//[EXCEPTION CASE: x <= y <= z]     ###left = y, middle = x, right = z### 
//
string* tab::SearchNShow(string right, string middle, string left, int &count/*, SEARCH_MODE SearchMode*/) {
	string *searchres = new string[100];
	searchres = Search(right, middle, left, count);

	int n;
	for (int i = 0; i < count; i++) {
		n = atoi(searchres[i].c_str());

		cout << "Search Success! The item is in Line " << n << endl;
		_line[n].show();
	}

	return searchres;
}

//
//Delete a key value using parsed data from a SQL.
//Parameter:
//key:the key need deleted.
//
bool tab::Delete(string right, string middle, string left/*, SEARCH_MODE SearchMode*/) {
	int count = 0;			//Record the count of item found.
	string* searchres = new string[100];
	int *seq = new int[100];
	
	searchres = Search(right, middle, left, count);
	
	//If the key is not in the data.
	if (count == 0) {
		cout << "Delete key not found!\n";
		return false;
	}

	//Convert searchres string to seq int.
	for (int i = 0; i < count; i++) 
		seq[i] = atoi(searchres[i].c_str());
	delete[] searchres;
	searchres = NULL;

	//Read the tab data.
	index _ins;
	string str = _ins.read(_tabPath);

	//Count the sharps.
	int i = 0, j = 0;
	int sharpCount = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '#')
			sharpCount++;
		i++;
	}
	
	int* sharp = new int[sharpCount]();
	i = 0;

	//Record the place of '#'.
	while (str[i] != '\0')
	{
		if (str[i] == '#')
			//cout << i << " ";
			sharp[j++] = i;

		i++;
	}
	
	//Get the string after delete.
	int begin = 0;
	int end = 0;
	for (int i = 0; i < count; i++) {
		begin = sharp[seq[i]];
		end = sharp[seq[i] + 1];

		str.erase(begin, end - begin);
	}
	
	delete[] sharp;
	delete[] seq;
	sharp = NULL;
	seq = NULL;

	//Rewrite the data into tab file.
	ofstream _OUTF;

	remove(_tabPath.c_str());
	
	_OUTF.open(_tabPath,ios::app);
	_OUTF << str;

	Initialize();
	cout << "Delete Success!\n";
	return true;
}

//
//Display the current table.
//
void tab::Display() {
	//Display the basic data of table.
	cout << "###############Display###############\n----------Basic Information----------\n";
	cout << "Table [" << _tabName << "]    ----->    Primary Key [" << _rawData._attrName[_rawData._PK_pos] << "]\n";
	
	//Display the raw attribute data.
	cout << "----------Raw Attribute Data---------\n";
	for (int i = 0; i < _rawData._attrCount; i++) {
		cout << "Attribute [" << _rawData._attrName[i] << "]    ----->    Restriction [" << _rawData._attrRstr[i] << "]\n";
	}
	
	//Display the raw attribute data.
	cout << "-----------Attribute Value-----------\n";
	for (int i = 0; i < _lineCount; i++) {
		cout << _line[i].raw<<endl;
	}
	cout << "#############Display Over#############\n";


}

//
//set the [_rawData] using the raw table in [_rawtabPath].
//
void tab::setRawdata(string* attrName, string* attrRstr, int attrCount, int PK_pos) {
	//Allocate the space for rawData data.
	_rawData._attrName = new string[attrCount];
	_rawData._attrRstr = new string[attrCount];
	
	//Assign value for rawData data.
	for (int i = 0; i < attrCount; i++)
	{
		_rawData._attrName[i] = attrName[i];
		_rawData._attrRstr[i] = attrRstr[i];
	}
	_rawData._attrCount = attrCount;
	_rawData._PK_pos = PK_pos;

}

//
// Insert a piece of line into the current table. 
//
bool tab::Insert(line &inLine) {	
	//Get the primary key position.
	index _ins;
	int PK = _rawData._PK_pos;	
	bool flag = false;
	inLine.split();

	//If the table has not been inserted , allocate [_line] a new space.
	if (_line == NULL) {
		_line = new line;
	}

	//Assign the inLine into the line of current table.
	_line[_lineCount++] = inLine;
	//_line[_lineCount - 1].split();

	//Btree insert.
	string value;
	stringstream ss;
	ss << inLine.seq;
	value = ss.str();

	PK_index.insert(inLine.item[PK + 1], value);

	//cout << PK_index.toStr();
	return true;
}

//
//Search for an item in an B-tree.
//Parameters:
//right: the right value of the expression.
//middle:the middle value of the expression.
//left:the left value of the expression.
//[EXCEPTION CASE: x <= y <= z]     ###left = y, middle = x, right = z### 
//
string* tab::Search(string right, string middle, string left, int &count/*, SEARCH_MODE SearchMode*/) {
	int indexPos = -1;						 //Store the left value index.
	string* searchres = NULL;			 //Store the searched sequence result.

										 //Search for the attribute position of the left value. 
	for (int i = 0; i < _lineCount; i++) {
		if (left == _rawData._attrName[i])
		{
			indexPos = i;
			break;
		}
	}

	//If the attribute of left value is not exist.
	if (indexPos == -1)
	{
		cout << "No such attribute!\n";
		return searchres;
	}

	stringstream ss;
	searchres = new string[100];
	//If the attribute of left value is the Primary Key position.
	if (indexPos == _rawData._PK_pos)
		PK_index.find(middle, searchres, count);

	//If the attribute of left value is exist but not the Primary Key position. 		
	if (indexPos != _rawData._PK_pos && indexPos != -1) {
		bTree searchBT;
		string key;
		string value;

		for (int i = 0; i < _lineCount; i++) {
			ss.str("");
			ss << i;

			key = _line[i].item[indexPos];
			value = ss.str();

			searchBT.insert(key, value);
		}
		searchBT.find(middle, searchres, count);
	}

	/*
	//Case: EQUAL
	if (SearchMode == 0) {
	tmpBT->searchBTree(tmpBT->getRoot(), midItem, res[0], sres);
	}

	//Case: NOT_EQUAL
	else if (SearchMode == 1) {


	}

	//Case: GREATER
	else if (SearchMode == 2) {


	}

	//Case: SMALLER
	else if (SearchMode == 3) {


	}

	//Case: REGION
	else if (SearchMode == 4) {


	}

	//Case: NO such SearchMode.
	else{
	cout << "No such SearchMode!\n";
	return res;
	}
	*/
	if (count == 0)
	{
		cout << "No such item!\n";
		return searchres;
	}


	return searchres;
}

//
// If the table has a inserted line, Initialize the class.
//
void tab::Initialize() {
	index _ins;
	int lineCount = 0;

	//Read the [.tab] file.
	ifstream _INF;
	_INF.open(_tabPath, ios::_Nocreate);
	string tmp;
	_INF >> tmp;
	_INF.close();

	//the current table has no inserted line
	if (tmp.length() == 1)
		return;

	for (int i = 1; i < tmp.length(); i++) {
		if (tmp[i] == '#')
			lineCount++;
	}
	_lineCount = lineCount;
	_line = _ins.parseToLine((char*)tmp.data(), lineCount);
	
	for(int i=0;i<lineCount;i++)
		_line[i].split();

	if (PK_index.isNULL())
	{
		//PK_index Btree initialization
		stringstream ss;
		string key;
		string value;

		for (int i = 0; i < _lineCount; i++) {
			key = _line[i].item[_rawData._PK_pos];
			
			ss << i;
			value = ss.str();
			ss.str("");

			PK_index.insert(key, value);
		}
		//cout<<PK_index.toStr();
	}

	//cout << "Initialize success!\n";
}