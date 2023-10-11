#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
ifstream fin;
ofstream fout;
class Matrix {
	int n;
	int** matr;
public:
	Matrix() {
		this->n = 0; 
		this->matr = 0;
	}
	Matrix(int N) {
		if (N <= 0) throw "Íåâåðíûé ðàçìåð";
		this->n = N;
		this->matr = new int* [N];
		
		for (int i = 0; i < N; i++) {
			this->matr[i] = new int[N];
			for (int j = 0; j < N; j++) {
				srand(time(NULL));
				this->matr[i][j] = rand();
			}
		}
	}
	Matrix(int N, int k) {
		if (N <= 0) throw "Íåâåðíûé ðàçìåð";
		this->n = N;
		this->matr = new int* [N];
		for (int i = 0; i < N; i++) {
			this->matr[i] = new int[N];
			for (int j = 0; j < N; j++) 
				this->matr[i][j] = 0;
			this->matr[i][i] = k;
		}
	}
	~Matrix() {
		for (int i = 0; i < this->n; i++) 
			delete[] this->matr[i];
		delete[] this->matr;
	}
	Matrix(const Matrix& that) {
		this->n = that.n;
		this->matr = new int*[this->n];
		this->n = that.n;
		for (int i = 0; i < this->n; i++) {
			this->matr[i] = new int [this->n];
			for (int j = 0; j < this->n; j++) {
				this->matr[i][j] = that.matr[i][j];
			}
		}
	}
	Matrix operator+(const Matrix& that) const{
		Matrix res(this->n);
		for (int i = 0; i < this->n; i++) {
			for (int j = 0; j < this->n; j++)
				res.matr[i][j] = this->matr[i][j] + that.matr[i][j];
		}
		return res;
	}
	Matrix operator*(const Matrix& that) const {
		Matrix res(this->n);
		int sum = 0;
		for (int i = 0; i < this->n; i++) {
			for (int j = 0; j < this->n; j++) {
				for (int k = 0; k < this->n; k++)
					sum += this->matr[i][k] * that.matr[k][j];
				res.matr[i][j] = sum;
				sum = 0;
			}
			
		}
		return res;
				
	}
	Matrix& operator=(const Matrix& that) {
		if (this != &that) {
			this->~Matrix();
			
			this->n = that.n;
			this->matr = new int* [this->n];
			for (int i = 0; i < this->n; i++) {
				this->matr[i] = new int[this->n];
				for (int j = 0; j < this->n; j++)
					this->matr[i][j] = that.matr[i][j];
			}
		}
		return *this;

	}
	
	
	void SetElem() {
		for (int i = 0; i < this->n; i++)
			for (int j = 0; j < this->n; j++)
				fin >> this->matr[i][j];
		
	}
        void Transpose() {
		int rem;
		for (int i = 0; i < this->n; i++) {
			for (int j = i + 1; j < this->n; j++) {
				rem = this->matr[i][j];
				this->matr[i][j] = this->matr[j][i];
				this->matr[j][i] = rem;

			}
		}
		
	}
	void PrintMatr() {
		for (int i = 0; i < this->n; i++) {
			for (int j = 0; j < this->n; j++)
				fout << this->matr[i][j] << " ";
			fout << endl;
		}
	}
};

int main() {
	int N;
	int k;
	fin.open("input.txt");
	fout.open("output.txt");
	fin >> N >> k;
	Matrix K(N, k);
	Matrix A(N);
	Matrix B(N);
	Matrix C(N);
	Matrix D(N);
	A.SetElem();
	B.SetElem();
	C.SetElem();
	C.Transpose();
	D.SetElem();
	D.Transpose(); 
	
	Matrix Res = (B * C + A + K)*D;
	Res.PrintMatr();
	
	fin.close();
	fout.close();
	return 0;
}
