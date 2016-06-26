#pragma once
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
class SQL
{
public:
	void select(string str, string s[]);
	void update(string str, string s[]);
	void createTab(string str, string col[], string type[], string& name);
	void insert(string str, string col[], string values[], string& name);
	void deleteValue(string str, string s[]);

	string sel[5] = { "SELECT","FROM","WHERE","GROUP BY","ORDER BY" };
	string upd[3] = { "UPDATE", "SET", "WHERE" };
	string del[3] = { "DELETE","FROM","WHERE" };
	string PK = "PRIMARY KEY";
};