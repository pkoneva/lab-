#include<ctime>
#include<windows.h>
#include <shellapi.h> 
#include<conio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<queue>
#include<string>
#include<vector>
#include<map>
#include <atomic>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<time.h>

#include<unordered_map>

using namespace std;
ifstream fin;
ofstream fout;

hash<string> adr_hash;
map<int, string > adr_map;
queue<string> adr_que;
map<int, string> visited;
mutex que;
mutex M;
mutex adr;
condition_variable que_is_not_empty;
bool done=false;
bool notified = false;


void crawler(string adr) {
	ifstream curr_file;
	string search = "<a href=\"file://";
	string str;
	const int len = search.length();
	string next_adr;

	while (!done) {
		string adrr;
		que.lock();
		if (!adr_que.empty())
			adrr = adr_que.front();
		else {
			que.unlock(); continue;
		}
		adr_que.pop();
		que.unlock();
		adrr.erase(0, 16);
		adrr.pop_back(); adrr.pop_back();
		adrr.insert(0, "C:\\test_data\\");
		curr_file.open(adrr, ios_base::in);

		if (curr_file.is_open()) {
			while (!curr_file.eof()) {
				getline(curr_file, str);
				bool end_str = false;
				while (!end_str) {
					int i = str.find(search);
					if (i != string::npos) {
						int j = i + len;

						next_adr = search;
						for (j; j < i + len + 3; j++) {
							if (str[j] >= '0' && str[j] <= '9')
								next_adr.push_back(str[j]);
							else break;
						}
						str.erase(0, j);
						if (str.find(".html\">") == 0) {
							next_adr += ".html\">";
							str.erase(0, 8);
						}
						M.lock();
						if (adr_map.find(adr_hash(next_adr)) == adr_map.end()) {
							adr_map.emplace(make_pair(adr_hash(next_adr), next_adr));
							que.lock();
							adr_que.emplace(next_adr);
							que.unlock();
						}
						M.unlock();
					}
					else end_str = true;
				}

			}
			curr_file.close();
			if (adr_que.empty()) done = true;
		}
	}

	return;
}
int main() {
	time_t start, end;
	fin.open("input.txt");
	fout.open("output.txt");
	
	string adr;
	int Count_Thr;
	getline(fin, adr);
	cin >> Count_Thr;
	time (&start);
	adr_que.emplace(adr);
	adr_map.emplace(make_pair(adr_hash(adr), adr));
	notified = true;
	vector <thread> thr;
	for (int i = 0; i < Count_Thr; i++) {
		thr.emplace_back(thread(crawler, adr));
	}
	for (int i = 0; i < Count_Thr; i++)
	{
		thr[i].join();
	}
	time (&end);
	std::cout <<"count: "<< adr_map.size()<< "\ntime: " << difftime(end, start);

}
