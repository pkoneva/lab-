#include <fstream>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <string> 
#include <iterator>
using namespace std;
ifstream fin;
ofstream fout;
template <typename K, typename T>

class List
{public:
	int hash;
	K k;
	T t;
	List<K, T>* next;
	~List() {};
	bool operator!=(List const& that) const { return t != that.t; }
	bool operator==(List const& that) const { return t == that.t; }
};
template <typename K, typename T>
class Ordered_List
{
	const int size = 10;
	
	
public:
	List<K, T>* top;
	int fullness;
	Ordered_List() {
		fullness = 0;
		top = NULL;
	}
	void Next() {
		static List<K, T>* l;
	}
	void Push(K kn, T tn, int h)
	{
		if (Empty()) {
			top = new List<K, T>;
			top->k = kn;
			top->t = tn;
			top->hash = h;
			fullness = 1;
			top->next = NULL;
		}
		else if (!Full()) {
			List<K, T>* l = new List<K, T>;
			l->k = kn;
			l->t = tn;
			top->hash = h;
			l->next = top;
			top = l;
			l->next = NULL;
			fullness++;
		}
	}
	void Pop() {
		if (!Empty()) {
			List<K, T>* l = top;
			top = top->next;
			T rem = l->t;
			delete(l);
			fullness--;
			return;
		}
		else return;
	}
	List<K, T>* find(K key) {
		List<K, T>* l = top;List<K, T>* p=l;
		while (l != NULL) {
			
			if (l->k == key)
				return l;
			p = l;
			l = l->next;
		}
		return p;
	}
	void find_del(K key) {
		List<K, T>* l = top;
		if (l == NULL) return;
		List<K, T>* p = l, *s=l;
		while (l != NULL) {
			if (l->next != NULL && l->k == key) {
				s = l;
				p = l->next;
				p->next = l;
				delete(p);
			}
			if (l == NULL) return;
		}
	}
	void find_change(K key, T val, int h) {
		List<K, T>* l = this->find(key);
		if (l != NULL) {
			if (l->k == key) {
				l->t = val; return;
			}
			List<K, T>* p = new List<K, T>;
			p->k = key;
			p->t = val;
			p->hash = h;
			l->next = p;
			p->next = NULL;
			fullness++;
		}
		else { this->Push(key, val, h); }
	}
	
	bool Empty() {
		if (top == NULL) return true;
		else return false;
	}
	bool Full() {
		if (fullness == size) return true;
		else return false;
	}
	List<K, T>* getNext(List<K, T>* l) {
		l= l->next ;
		return l;
	}

	void setNext(List<K, T>* l) {
		l->next = l;
	}
	bool operator!=(Ordered_List const& that) const { return top->t != that.top->t; }
	bool operator==(Ordered_List const& that) const { return top->t == that.top->t; }
	
};

template <typename K, typename T>
class HashMap {
	int size = 3;
	Ordered_List<K, T>* table;
	
	float loadFactor = 0.75; //êîýôôèöèåíò çàãðóçêè
public:
	float limitElem = size * loadFactor;
	int full = 0; //ïðåäåëüíîå êîëè÷åñòâî ýëåìåíòîâ, ïðè äîñòèæåíèè êîòîðîãî óâåëè÷èâàåì õýøòàáëèöó
	void function();
	HashMap() {
		full = 0;
		table = new Ordered_List<K, T>[size];
		for (int j = 0; j < size; j++) table[j].top = NULL;
	}
	HashMap(int n) {
		full = 0;
		size *= 2*n;
		table = new Ordered_List<K, T>[size];
		for (int j = 0; j < size; j++) table[j].top = NULL;
	}
	hash<K> hashK;
	int GetInd(K nk) {
		int h = hashK(nk);
		int i = abs(h % size);
		return i;
	}
	void PutElem(K nk, T nt) {
		int h = hashK(nk);
		int i = abs(h % size);
		full -= table[i].fullness;
		if (table[i].top != NULL) {
			table[i].find_change(nk, nt, h);
		}
		else table[i].Push(nk, nt, h);
		full += table[i].fullness;
	}
	void Del(K del_k) {
		int i = GetInd(del_k);
		if (table[i].top == NULL) return;
		if (table[i].top->next != NULL) {
			table[i].find_del(del_k);
		}
		else table[i].Pop();
		full--;
	}
	T Find(K k) {
		int i = GetInd(k);
		return table[i].find(k)->t;
	}
	~HashMap() {
		delete(table);
	}
		
	//øàáëîí èòåðàòîðà èç áèáëèîòåêè
	 class iterator :public std:: iterator<input_iterator_tag,  //òèï èòåðàòîðà èç áèáëèîòåêè
		HashMap<K, T>, ptrdiff_t, HashMap<K, T>*, HashMap<K, T>&> {

		HashMap<K, T>* ht;
		int ind=-1;
		
	public:
		int Complete = 0;
		iterator(HashMap<K, T>* H) {
			ht = H;
			ind = 0;
		}
		iterator(HashMap<K, T>* H, int i) {
			ht = H;
			ind = i;
		}
		
		Ordered_List<K, T>* currList;
		List<K, T>* l;
		iterator operator*() { return *this;}
		List<K, T> operator->() { return *ht->table[ind].top; }
		bool operator!=(iterator const& that) { return (ht->table[ind].top != that.ht->table[ind].top ||ind!=that.ind); }
		bool operator==(iterator const& that) { return ht->table[ind].top == that.ht->table[ind].top && ind == that.ind; }
		
		void SearchNextList(int i)
		{
			if (i >= ht->size) {
				Complete = 1;
				return;
			}
			int j;
			for (j = i; j < ht->size; j++)
				if (!ht->table[j].Empty())
				{
					currList = &ht->table[j];//ñëåäóþùèé íåïñóòîé ñïèñîê
					ind = j;//è åãî íîìåð
					return;
				}
			
			Complete = 1;
			return;
		}
		iterator& operator++() {
			iterator it = *this;
			
			if (it.ind == -1 ||  (l!=NULL && l->next==NULL) || it.currList == NULL || (it.currList!=NULL &&it.currList->Empty())) {
				it.SearchNextList(it.ind == -1 ? 0 : it.ind+1);
				if (it.currList == NULL) {
					Complete = 1;
				}
				else it.l = it.currList->top;
			}
			else it.l=it.currList->getNext(l);
			*this = it;
			return *this;
		}

	};
	iterator begin()
	{
		return iterator(this,-1);
	}

	iterator end()
	{
		return iterator(this,size);
	}
	void NewTable(int rise) {
		iterator it = begin();
		HashMap<K, T>* newht = new HashMap<K, T>(rise);
		
		for (int i=0; i < newht->size; i++) { 
			newht->table[i].top = NULL; }
		if (table[0].top == NULL) ++it;
		else it.l= table[0].top;
			while (it != end())
			{
				K key= it.l->k;
				T val = it.l->t;
				newht->PutElem(key, val);
				++it;
				if (it.Complete == 1) break;
			}
			this->table = newht->table;
			this->size = newht->size;
			this->limitElem = size * loadFactor;
	}
	
};

template <typename K, typename T> void HashMap<K, T>::function() {
	K key; 
	T val;
	int N;
	char command;
	fin >> N;
	int count_rise = 0;//êîëè÷åñòâî ðàñøèðåíèé õýø-òàáëèö
	for (int i = 0; i < N; i++) {
		fin >> command;
		if (command == 'A') {
			fin >> key >> val; PutElem(key, val); 
		}
		if (command == 'R') { fin >> key; Del(key);  }
		if (full >= limitElem) {
			count_rise++;
			NewTable(count_rise);
		}
	}
	if (full == 0) {
		fout << full << " " << full; return;
	}
	//ïîäñ÷¸ò êîëè÷åñòâà ýëåìåíòîâ
	iterator it = begin();
	iterator iend = end();
	int n = 0;
	while (it != iend)
	{
		++it;
		if (it.Complete == 1) break;
		n++;
	}
	fout << n << " ";
	//ïîäñ÷¸ò êîëè÷åñòâà ðàçëè÷íûõ çíà÷åíèé
	if (n == 0) {
		fout << n; return;
	}
    List<K, T>* arr = new List<K, T>[n];
	iterator newit = begin();
	int i = 0;
	++newit;
	arr[i] = *newit.l;
	int flag = 0;
	for (++i;newit != iend; )
	{
		if (newit.Complete == 1) break;
		flag = 0;
		for (int j = 0; j < i; j++) {
			if (arr[j].t == newit.l->t) {
				flag = 1;
				break;
			}
		}
		if (flag==0) {
			arr[i++] = *newit.l;
		}
		++newit;
	}
	fout << i;
	return;
}
int main() {
	fin.open("input.txt");
	fout.open("output.txt");
	char type_key, type_val;
	
	fin >>  type_key >> type_val;
	if (type_key == 'I') {
		if (type_val == 'I') { HashMap<int, int> HM; HM.function(); }
		else if (type_val == 'D') {
			HashMap<int, double> HM;  HM.function();
		}
		else {
			HashMap<int, string> HM;  HM.function();
		}
	}
	else if (type_key == 'D') {
		if (type_val == 'I') {
			HashMap<double, int> HM;  HM.function();
		}
		else if (type_val == 'D') {
			HashMap<double, double> HM;  HM.function();
		}
		else {
			HashMap<double, string> HM;  HM.function();
		}
	}
	else {
		if (type_val == 'I') {
			HashMap<string, int> HM;  HM.function();
		}
		else if (type_val == 'D') {
			HashMap<string, double> HM;  HM.function();
		}
		else {
			HashMap<string, string> HM;  HM.function();
		}
	}
	
	fin.close();
	fout.close();
	return 0;
}
