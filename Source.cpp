#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;
ifstream fin;
ofstream fout;


class Expression {
protected:
public:
	virtual void print() = 0;
	virtual Expression* diff() = 0;
};

class Number : public Expression {
	double n;
public:
	Number() { n = 0; }
	Number(double x) { n = x; }
	Expression* diff() { return new Number(); }
	void print() { fout << this->n; }
};

class Variable : public Expression {
	char c;
public:
	Variable() { throw "¬ведите переменную"; }
	Variable(char x) { c = x; }
	Expression* diff() { return new Number(1); }
	void print() { fout << this->c; }
};

class Add : public Expression {
	Expression* e1, * e2;
public:
	Add() { throw "¬ведите выражение"; }
	Add(Expression* a, Expression* b) {
		e1 = a; e2 = b;
	}
	Expression* diff() {
		return new Add(e1->diff(), e2->diff());
	}
	void print() {
		fout << "(";
		e1->print();
		fout << "+";
		e2->print();
		fout << ")";
	}
};

class Sub : public Expression {
	Expression* e1, * e2;
public:
	Sub() { throw "¬ведите выражение"; }
	Sub(Expression* a, Expression* b) {
		e1 = a; e2 = b;
	}
	Expression* diff() {
		return new Sub(e1->diff(), e2->diff());
	}
	void print() {
		fout << "(";
		e1->print();
		fout << "-";
		e2->print();
		fout << ")";
	}
};

class Mul : public Expression {
	Expression* e1, * e2;
public:
	Mul() { throw "¬ведите выражение"; }
	Mul(Expression* a, Expression* b) {
		e1 = a; e2 = b;
	}
	Expression* diff() {
		return new Add(new Mul(e1->diff(), e2), new Mul(e1, e2->diff()));
	}
	void print() {
		fout << "(";
		e1->print();
		fout << "*";
		e2->print();
		fout << ")";
	}
};

class Div : public Expression {
	Expression* e1, * e2;
public:
	Div() { throw "¬ведите выражение"; }
	Div(Expression* a, Expression* b) {
		e1 = a; e2 = b;
	}
	Expression* diff() {
		return new Div(new Sub(new Mul(e1->diff(), e2), new Mul(e2->diff(), e1)), new Mul(e2, e2));
	}
	void print() {
		fout << "(";
		e1->print();
		fout << "/";
		e2->print();
		fout << ")";
	}
};

int Perem(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
	return 0;
}
double Find_Number(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c == '.') return 0.1;
	return -10000000000;
}
int priority_znak(char c) {
	if (c == '*') return 1;
	if (c == '+') return 2;
	if (c == '/') return 3;
	if (c == '-') return 4;
	return -1;
}


struct List {
	char znak;
	Expression* e;
	List* next;
	~List() {};
};


class Stack {
	List* top;
public:
	Stack() {
		top = NULL;
	}
	void p_Push(char c) {
		List* q = new List;
		q->znak = c;
		q->e = NULL;

		if (top== NULL) { top = q; top->next = NULL; return; }
		q->next = top;

		top = q;
	}
	void e_Push(Expression* expr) {
		List* q = new List;
		q->e = expr;
		q->znak = EOF;
		if (top == NULL) { top = q; top->next = NULL; return; }
		q->next = top;
		top = q;
	}
	char p_Pop() {
		if (top== NULL) return EOF;
		if (top->znak != EOF) {
			char c = top->znak;
			List* p = top;
			top = p->next;
			delete(p);
			return c;
		}

	}
	Expression* e_Pop() {
		if (top != NULL && top->e != NULL) {
			Expression* r = top->e;
			List* p = top;
			top = top->next;
			delete(p);
			return r;
		}
		return 0;
	}

	~Stack() {
		while (top != NULL) {
			List* p = top;
			top = top->next;
			delete(p);
		}
	}

	Expression* create();
	void composite();
};
Stack st_oper;
Stack st_expr;

void Stack::composite() {
	if (st_oper.top== NULL) return;
	int zn = priority_znak(st_oper.p_Pop());
	if (zn == -1) return;
	Expression* e2 = st_expr.e_Pop();// выталкиваетс€ в обратном пор€дке
	Expression* e1 = st_expr.e_Pop();
	Expression* res = NULL;
	switch (zn) {
	case 1: {res = new Mul(e1, e2); st_expr.e_Push(res); return; }
	case 2: {res = new Add(e1, e2); st_expr.e_Push(res); return; }
	case 3: {res = new Div(e1, e2); st_expr.e_Push(res); return; }
	case 4: {res = new Sub(e1, e2); st_expr.e_Push(res); return; }
	}

}
Expression* Stack::create() {
	char c;
	Expression* en;
	while (fin >> c) {

		double num, n_help;
		if ((num = Find_Number(c)) != -10000000000) {

			int zpt = (num == 0.1 ? 1 : 0);
			while (fin>>c && (n_help = Find_Number(c)) != -10000000000 ) {
				if (zpt != 0) zpt++;
				if (n_help == 0.1) {
					zpt = 1;
					fin >> c;
					n_help = Find_Number(c);
					num += 0.1 * n_help;
				}
				else if (zpt >= 1) {
					for (int i = zpt; i > 0; i--) n_help *= 0.1;
					num += n_help;
				}
				else {
					num *= 10;
					num += n_help;
				}
			}
			en = new Number(num);
			st_expr.e_Push(en);
			int long_file = fin.tellg(); //считан лишний символ, необходимо передвинуть текущую позицию на 1 байт назад
			fin.seekg(long_file-1, ios_base::beg);
		}
		else if (Perem(c) == 1) {
			en = new Variable(c);
			st_expr.e_Push(en);
		}
		else if (c == '(') st_oper.p_Push(c);
		else if (c == ')') {
			while (st_oper.top!= NULL && st_oper.top->znak != '(') {
				st_oper.composite();
			}
			if (st_oper.top == NULL) continue;
			st_oper.p_Pop();
		}

		else {
			while (st_oper.top!= NULL && priority_znak(c) <= priority_znak(st_oper.top->znak)) {
				st_oper.composite();
			}
			st_oper.p_Push(c);
		}
	}
	while (st_oper.top!= NULL) { composite(); }
	return st_expr.e_Pop();
}

int main() {
	fin.open("input.txt");
	fout.open("output.txt");
	Expression* e;
	e = st_oper.create();
	Expression* de = e->diff();
	de->print();
	fin.close();
	fout.close();
	return 0;
}