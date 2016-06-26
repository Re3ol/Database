#include <stdio.h>
#include "DBManager.h"

using namespace std;

static void operator_overload_test(sItem &s1, sItem &s2);//Test function of operator overload.

void main() 
{	
	//index and line test.
	/*
	index _ins;
	line* s ;
	int _lineCount;
	sItem * _1s;

	//Read into data.
	char* data = _ins.read("C:\\Users\\Ã¿æ∞Ë∫\\Desktop\\data.txt");	 
	
	//Parse into s.
	s = _ins.parseToLine(data, _lineCount);							 
	
	 //Split each line.
	for (int i = 0; i < _lineCount; i++)							
		s[i].split();
	
	//get column of s in the 1st position.
	_1s = _ins.getColumn(s, _lineCount, 1);								
	*/
	//B-tree test
	/*
	sItem* test = new sItem[7];
	index _ins;
	bTree bt(3);

	test[0] = sItem("Tom", 0);
	test[1] = sItem("Jerry", 1);
	test[2] = sItem("Kate", 2);
	test[3] = sItem("Tom", 3);
	test[4] = sItem("Rick", 4);
	test[5] = sItem("Dick", 5);
	test[6] = sItem("Tom", 6);

	//Insert
	string keyIns;
	string valIns;
	for (int i = 0; i < 7; i++) {
		_ins.convert(test[i], keyIns, valIns);
		bt.insert(keyIns, valIns);
	}
	cout << bt.toStr()<<endl;
	
	//Search
	string keySel = "Tom";
	string *valSel = new string[100];
	int numSel = 0;

	bt.find(keySel,valSel,numSel);

	for(int i = 0 ; i < numSel ; i++)
		cout<<valSel[i]<<endl;
	
	//Delete
	string keyDel;
	string valDel;
	bt.delete_key("Kate");
	cout<<bt.toStr()<<endl;
	*/
	//sItem OPERARTOR test
	/*
	sItem s1, s2;
	s1 = "Hello world";
	s2 = "world hello";
	s1.seq = 0;
	s2.seq = 1;
	//test for operator<
	operator_overload_test(s1,s2);
	s1 = "zero point.";
	//test for operator>
	operator_overload_test(s1, s2);
	//test for opeartor == & =
	s1 = s2;
	operator_overload_test(s1, s2);
	*/
	//Table main function test
	//Create and Insert test.		
	tab student;
	string attr[3] = { "id","name","addr" };
	string rstr[3] = { "PK","NULL","NULL" };
	
	string ins[3] = { "id", "name","addr"};
	rawdataTab insert(ins, 3);
	string values[3] = { "200", "Deric","Somali"};
	string _values[3] = { "102","Rick","Paris" };

	//char* testchar = "#|0|123|Tom|Shanghai|";
	//line testLine(21, 0, testchar);
	//testLine.split();
	if (student.Create("student", attr, rstr, 3))
		student.Display();
	
	//student.Insert(testLine);
	//student.Initialize();
	if(student.Insert(insert, values))
		student.Display();
	//if (student.Insert(insert, _values))
	//	student.Display();

	//Search test
	string searchItem = "Tom";
	string searchAttr = "name";
	int searchCount = 0;
	string* res;
	res = student.SearchNShow("", searchItem, searchAttr, searchCount);

	//Delete test
	string deleteItem = "127";
	string deleteAttr = "id";
	string _deleteItem = "130";
	if (student.Delete("", deleteItem, deleteAttr))
		student.Display();
	//if (student.Delete("", _deleteItem, deleteAttr))
	//student.Display();

	//Display test
	//student.Display();
	

	//SQL test
/*
	string str = "";
	string tmp;
	SQL sql;
	while (getline(cin, str)) {
		string s[100], col[100], type[100], name;
		tab student;
		string rstr[3] = { "PK","NULL","NULL" };
		switch (str[0])
		{
		case 'C':
			sql.createTab(str, col, type, name);
			cout << name << endl;
			for (int i = 0; i < 30; i++) {
				if (col[i] == "") break;
				cout << col[i] << "\t" << type[i] << endl;
			}
			break;
		case 'S':
			sql.select(str, s);
			for (int i = 0; i < 5; i++) {
				if (s[i].length() != 0) {
					cout << sql.sel[i] << ":\t" << s[i] << endl;
				}
			}
			break;
		case 'U':
			sql.update(str, s);
			for (int i = 0; i < 3; i++) {
				if (s[i].length() != 0) {
					cout << sql.upd[i] << ":\t" << s[i] << endl;
				}
			}
			break;
		case 'D':
			sql.deleteValue(str, s);
			for (int i = 0; i < 3; i++) {
				if (s[i].length() != 0) {
					cout << sql.del[i] << ":\t" << s[i] << endl;
				}
			}
			break;
		case 'I':
			sql.insert(str, col, type, name);
			cout << name << endl;
			for (int i = 0; i < 20; i++) {
				if (col[i] == "") break;
				cout << col[i] << "\t" << type[i] << endl;
			}
			break;
		default:
			break;
		}
}
*/
	system("pause");
}

//
//Test function of operator overload.
//
static void operator_overload_test(sItem &s1, sItem &s2){
	if (s1 > s2)
		cout << "[" << s1 << "] bigger than [" << s2 << "]"<<endl;
	else if (s1 < s2)
		cout << "[" << s1 << "] smaller than [" << s2 << "]" << endl;
	else
		cout << "[" << s1 << "] equals to [" << s2 << "]" << endl;
}


 