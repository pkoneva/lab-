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

//thread_local FILE* curr_file;
//thread_local ifstream curr_file;
//atomic<map<int, string>> adr_map;
//atomic<queue<string>> adr_que;
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

//" <a href="file://""




void crawler(string adr) {
	/*thread_local*/ ifstream curr_file;
	string search = "<a href=\"file://";
	/*thread_local*/ string str;
	const int len = search.length();
	/*thread_local*/ string next_adr;

	/*unique_lock<mutex> locker(que);*/
	//прочитать полностью один файл, поместить в очередь все ссылки, другие потоки ждут, когда по€витс€ перва€ ссылка в очереди и заюирают еЄ (условна€ перемнна€+noticed_one). 
	//уже прочитанный файл отправл€етс€ в хэш-таблицу/либо любой найденный, тогда повтор€ющиес€ ссылки не помещаютс€ в очередь
	while (!done) {
		string adrr;
		//adr_map; adr_que; while (!notified) {
		//	while(!adr_que.empty()) que_is_not_empty.wait(locker);
		//	/*if (!adr_que.empty()) { notified = true; que_is_not_empty.notify_one(); }
		//	else done = true;*/
		//}
		/*while ((!done)& notified) {*/
			/*if ((adr_map.find(adr_hash(adr_que.front())) != adr_map.end())) {
				adr_que.pop();
				continue;
			}*/
			/*if (adr_que.empty()) {
				done = true; continue;
			}*/

		que.lock();
		if (!adr_que.empty())
			adrr = adr_que.front();
		else {
			que.unlock(); continue;
		}
		adr_que.pop();
		que.unlock();
		/*if (visited.find(adr_hash(adrr)) == visited.end())
			visited.emplace(make_pair(adr_hash(adrr), adrr));
		else  continue;*/


		adrr.erase(0, 16);
		adrr.pop_back(); adrr.pop_back();
		adrr.insert(0, "C:\\test_data\\");
		/*system("C:\test_data\251.html");*/
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
							/*notified = true;


							que_is_not_empty.notify_one();*/
						}
						M.unlock();
						//int len = curr_file.tellg();
						//str.seekg(len - 16);//search длины 17, если вдруг строка вошла не полностью, будет возможность еЄ найти при втором считывании
						//continue;
					}
					else end_str = true;
				}

			}
			curr_file.close();
			/*}*///добавить копирование файлов и условие завершени€ работы
			if (adr_que.empty()) done = true;
		}
	}

	return;
}
int main() {
	time_t start, end;
	fin.open("input.txt");
	fout.open("output.txt");
	
	
	/*system("mkdir C : \forcopy");*/
	string adr;
	int Count_Thr;
	getline(fin, adr);
	cin >> Count_Thr;
	time (&start);
	adr_que.emplace(adr);
	adr_map.emplace(make_pair(adr_hash(adr), adr));
	notified = true;
	vector <thread> thr/*(Count_Thr, thread (crawler, adr))*/;
	for (int i = 0; i < Count_Thr; i++) {
		thr.emplace_back(thread(crawler, adr));
	}
	
	for (int i = 0; i < Count_Thr; i++)
	{
		thr[i].join();
	}
	//while (!done) {
	//	if (!adr_que.empty()) {  que_is_not_empty.notify_one(); notified = true;}
	//	/*else done = true; */
	//}
	/*for (int i = 0; i < Count_Thr; i++) {
		thread thr[i](crawler, adr);
	}*/

	/*thread thr(crawler, adr, Count_Thr);
	thr.join();*/
	/*system("RD /S C:\forcopy");*/
	time (&end);
	std::cout <<"count: "<< adr_map.size()<< "\ntime: " << difftime(end, start);
	/*return 0;*/
}