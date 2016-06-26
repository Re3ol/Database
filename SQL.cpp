#include"SQL.h"

void SQL::select(string str, string s[]) {
	int pos = 0;
	int l = str.length();
	int i = 0, a = 0;
	str = str.substr(0, str.find(";"));
	while (i < 5) {
		if (i == 0) {
			while (str.find(sel[i]) == -1) i++;
			str = str.substr(str.find(sel[i]) + sel[i].length() + 1);
			a = i;
			i++;
		}
		if (i == 5) {
			s[a] = str.substr(0, l);
			return;
		}
		while (str.find(sel[i]) == -1) i++;
		s[a] = str.substr(0, str.find(sel[i]) - 1);//s������ؼ��ʶ�Ӧ��λ�ô������
		if (s[a].find(";") != -1) {
			s[a] = s[a].substr(0, s[a].find(";"));
		}
		pos = str.find(sel[i]) + sel[i].length() + 1;
		str = str.substr(pos, l);
		a = i;//ʹs������sel�������Ӧ��
	}
	//ִ�в�ѯ����
}

void SQL::update(string str, string s[]) {
	int pos = 0;
	int l = str.length();
	int i = 0, a = 0;
	str = str.substr(0, str.find(";"));
	while (i < 3) {
		if (i == 0) {
			while (str.find(upd[i]) == -1) i++;
			str = str.substr(str.find(upd[i]) + upd[i].length() + 1);
			a = i;
			i++;
		}
		if (i == 3) {
			s[a] = str.substr(0, l);
			return;
		}
		while (str.find(upd[i]) == -1) i++;
		s[a] = str.substr(0, str.find(upd[i]) - 1);//s������ؼ��ʶ�Ӧ��λ�ô������
		if (s[a].find(";") != -1) {
			s[a] = s[a].substr(0, s[a].find(";"));
		}
		pos = str.find(upd[i]) + upd[i].length() + 1;
		str = str.substr(pos, l);
		a = i;//ʹs������upd�������Ӧ��
	}
	//ִ�и��²���
}

void SQL::createTab(string str, string col[], string type[], string& name) {
	int l = str.length();
	str = str.substr(str.find("C") + 13, l);
	name = str.substr(0, str.find_first_of(" "));
	int start = str.find_first_of("(") + 1;
	int end = str.find_last_of(")");
	str = str.substr(start, end - start - 1);
	int pos = str.find_first_not_of(" ");
	str = str.substr(pos, l);
	int count = 1;
	while (true)
	{
		if (count % 2 != 0) {
			pos = str.find_first_of(" ");
			col[(count + 1) / 2] = str.substr(0, pos);
			str = str.substr(pos + 1, l);
		}
		else {
			pos = str.find_first_of(",");
			type[count / 2] = str.substr(0, pos);
			str = str.substr(pos + 1, l);
			pos = str.find_first_not_of(" ");
			str = str.substr(pos, l);
		}
		count++;
		if (str.find(PK) == 0) {
			col[0] = str.substr(str.find_first_of("(") + 1, str.find_first_of(")") - 1 - str.find_first_of("("));
			type[0] = PK;
			break;
		}
	}
	//ִ�����ɱ�Ĳ���
}

void SQL::insert(string str, string col[], string values[], string& name) {
	int l = str.length();
	str = str.substr(str.find("INSERT INTO ") + 12, l);
	name = str.substr(0, str.find_first_of("("));
	string colList = str.substr(str.find_first_of("(") + 1, str.find_first_of(")") - str.find_first_of("(") - 1);
	str = str.substr(str.find("VALUES"), l);
	string valueList = str.substr(str.find_first_of("(") + 1, str.find_first_of(")") - str.find_first_of("(") - 1);
	int i = 0, l1 = colList.length(), l2 = valueList.length();
	while (true) {
		if (colList.find(",") !=-1) {
			col[i++] = colList.substr(colList.find_first_of("'") + 1, colList.find_first_of(",")-1 - colList.find_first_of("'") - 1);
			colList = colList.substr(colList.find_first_of(",") + 1, l1);
		}
		else {
			col[i] = colList.substr(colList.find_first_of("'") + 1, colList.find_last_of("'")-1 - colList.find_first_of("'"));
			break;
		}
	}
	i = 0;
	while (true) {
		if (valueList.find(",") != -1) {
			values[i++] = valueList.substr(valueList.find_first_of("'") + 1, valueList.find_first_of(",")-1 - valueList.find_first_of("'") - 1);
			valueList = valueList.substr(valueList.find_first_of(",") + 1, l2);
		}
		else {
			values[i] = valueList.substr(valueList.find_first_of("'") + 1, valueList.find_last_of("'")-1 - valueList.find_first_of("'"));
			break;
		}
	}
	//ִ�в������ݲ���
}
//INSERT INTO tab1('','','') VALUES('','','');

void SQL::deleteValue(string str, string s[]) {
	int pos = 0;
	int l = str.length();
	int i = 0, a = 0;
	str = str.substr(0, str.find(";"));
	while (i < 3) {
		if (i == 0) {
			while (str.find(del[i]) == -1) i++;
			str = str.substr(str.find(del[i]) + del[i].length() + 1);
			a = i;
			i++;
		}
		if (i == 3) {
			s[a] = str.substr(0, l);
			return;
		}
		while (str.find(del[i]) == -1) i++;
		s[a] = str.substr(0, str.find(del[i]) - 1);//s������ؼ��ʶ�Ӧ��λ�ô������
		if (s[a].find(";") != -1) {
			s[a] = s[a].substr(0, s[a].find(";"));
		}
		pos = str.find(del[i]) + del[i].length() + 1;
		str = str.substr(pos, l);
		a = i;//ʹs������upd�������Ӧ��
	}
	//ִ��ɾ������
}
//DELETE A FROM TAB1 WHERE C;