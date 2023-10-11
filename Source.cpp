#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
ifstream fin;
ofstream fout;

//enum States
//{
//	Start,
//	End,
//	Skip
//};
//enum Events
//{
//	Sign,
//	Unknown
//};
//class DKA {
//
//	States Scurr, Snext;
//	Events e;
//public:
//	DKA() {
//
//	}
//	bool AddTransition(States from, Events val, States to) {
//
//	};
//};

struct TableElem {
	int curr;
	int next;
	char sign;
	bool start;
	bool finish;
	TableElem() {
		curr = 0;
		next = 0;
		sign = EOF;
		start = false;
		finish = false;
	}
	~TableElem() {};
};

class DKA {
	int N;
	int k;
	int f;
	int* end;
	TableElem* Table;
public:
	int p;

	DKA() {
		fin >> N >> k>>f;
		end = new int[f];
		for (int i = 0; i < f; i++)
			fin >> end[i];
		fin >> p;
		Table = new TableElem[p];
	}
	void Create_TableElem() {
		for (int i = 0; i < p; i++) {
			fin >> Table[i].curr >> Table[i].next >> Table[i].sign;
			if (Table[i].curr == k) Table[i].start = true;
			for (int j = 0; j < f; j++)
				if (Table[i].curr == end[j]) Table[i].finish = true;
		}
	}
	bool check_start() {};
	bool check_finish() {};
	bool chek_transition() {};
	bool Chek_string() {
		int T;
		string s;
		fin >> T;
		for (int i = 0; i < T; i++) {
			fin >> s;
			
		}
	}
	~DKA() {
		delete(end);
		delete(Table);
	}
};


int main() {
	int N, k, f, p, T;
	char val;
	string s;









	fin.close();
	fout.close();
	return 0;
}