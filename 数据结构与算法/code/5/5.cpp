#include<bits/stdc++.h>
#include<vector>
using namespace std;


// 1、arratList -> vector
// 2、reload operator << and >> : T -> int 
// 3、reSet -> resize(num): If vector's size < num, it will extend though fill 0 element 
// 4、rows -> Rows cols -> Cols


template <class T>
struct matrixTerm 
{
	int row, col;
    T val;

    operator T() const {return val;}
    // type conversion from matrixTerm to T
    bool operator < (const matrixTerm &A) {
    	if (col == A.col) return row < A.row;
    	return col < A.col;
	}
};

template<class T>
class sparseMatrix {
	// 友元函数
    friend ostream& operator << (ostream&, sparseMatrix<int>&);
    friend istream& operator >> (istream&, sparseMatrix<int>&);
    
	public:
		void transpose(sparseMatrix<T> &b);					// O(1e4)
		int add(sparseMatrix<T> &b, sparseMatrix<T> &c);	// O(2e2)
		int Mul(sparseMatrix<T> &, sparseMatrix<T> &); 		
		int Mul_(sparseMatrix<T> &, sparseMatrix<T> &);		// O(1e6)   // P * Q
		void in();					// 重置
	    void SortbyCol();			// 列主次 prepared for Q
	
	private:
		int Rows, Cols;
		vector<matrixTerm<T> > terms;
}; 

// sparseMatrix's method achieve
// output
// explict code tooverload with T = int for test as compiler
// unable to generate
ostream& operator << (ostream& out, sparseMatrix<int>& x)
{// Put x in output stream.

   // put matrix characteristics
	out << x.Rows << " " << x.Cols  << '\n';

	vector<matrixTerm<int> >::iterator it = x.terms.begin();
	for (int i = 1; i <= x.Rows; i++) {
		for (int j = 1; j <= x.Cols; j++) {
			if (i == it -> row && j == it -> col && it != x.terms.end()) {
				out << it -> val;
				it ++;
			}
			else out << 0;
			if (j == x.Cols) out << " \n";
			else out << " ";
		}
	}
	return out;
}

istream& operator >> (istream& in, sparseMatrix<int>& x)
{// Input a sparse matrix.

   // input matrix characteristics
	int numberOfTerms;
	in >> x.Rows >> x.Cols >> numberOfTerms;
   // set size of x.terms and ensure enough capacity
    x.terms.resize(0);	// numberOfTerms
	x.terms.clear();
	
   // input terms
    matrixTerm<int> mTerm;
    for (int i = 0; i < numberOfTerms; i++)
    {
        in >> mTerm.row >> mTerm.col >> mTerm.val;
	  	x.terms.push_back(mTerm);
    }
    return in;
}

template<class T>
void sparseMatrix<T>::transpose(sparseMatrix<T> &b)
{// Return transpose of *this in b.

	// set transpose characteristics
	b.Cols = Rows;
	b.Rows = Cols;
	b.terms.resize(terms.size());
	
	// initialize to compute transpose
	int* colSize = new int[Cols + 1];
	int* rowNext = new int[Cols + 1];
	
	// find number of entries in each column of *this
	for (int i = 1; i <= Cols; i++) // initialize
		colSize[i] = 0;  
	
	// 类似桶排序过程 
	for (vector<matrixTerm<int> >::iterator i = terms.begin(); i != terms.end(); i++)
		colSize[i -> col]++;  
	
	// find the starting point of each row of b
	rowNext[1] = 0;
	for (int i = 2; i <= Cols; i++){
		rowNext[i] = rowNext[i - 1] + colSize[i - 1]; 
	}
	/* 1 0 0 1
	   0 1 0 1
	   1 1 0 0
	   0 0 0 0 */
	
	// perform the transpose copying from *this to b
	// 确定转置之后的映射关系, 直接set 
	matrixTerm<T> mTerm;
	for (vector<matrixTerm<int> >::iterator i = terms.begin();	i != terms.end(); i++)
	{
		int j = rowNext[i -> col]++; // position in b
		mTerm.row = (*i).col;
		mTerm.col = (*i).row;
		mTerm.val = (*i).val;
		//	  b.terms.set(j, mTerm);
		b.terms[j] = mTerm;
	}
/* 3 1 4 2 5 9 */  
}

template<class T>
int sparseMatrix<T>::add(sparseMatrix<T> &b, sparseMatrix<T> &c)
{// Compute c = (*this) + b.

	// verify compatibility
	if (Rows != b.Rows || Cols != b.Cols) {
		// P = Q
		return -1;
	}
	//     throw matrixSizeMismatch(); // incompatible matrices
	
	// set characteristics of result c
	c.Rows = Rows;
	c.Cols = Cols;
	c.terms.clear();
	
	// define iterators for *this and b
	vector<matrixTerm<int> >::iterator it = terms.begin();
	vector<matrixTerm<int> >::iterator ib = b.terms.begin();
	vector<matrixTerm<int> >::iterator itEnd = terms.end();
	vector<matrixTerm<int> >::iterator ibEnd = b.terms.end();
	
	// move through *this and b adding like terms
	while (it != itEnd && ib != ibEnd)
	{
	 // row-major index plus cols of each term
		int tIndex = (*it).row * Cols + (*it).col;
		int bIndex = (*ib).row * Cols + (*ib).col;
	
		if (tIndex < bIndex)
		{// b term comes later
			c.terms.push_back(*it); 
			it++;
		}
		else {
			if (tIndex == bIndex)
	    	{// both in same position
	
	          // append to c only if sum not zero
	        	if ((*it).val + (*ib).val != 0)		// 这里约束了 matrixterm 中的 val 为 int 类型
	        	{
	            	matrixTerm<T> mTerm;
	            	mTerm.row = (*it).row;
	            	mTerm.col = (*it).col;
	            	mTerm.val = (*it).val + (*ib).val;
					c.terms.push_back(mTerm); 
	        	}
	
	        	it++; 
	        	ib++;
	       }
	       else
	       {// a term comes later
				c.terms.push_back(*ib);
	        	ib++;
	       }
	    }
	}
	
	// copy over remaining terms
	for (; it != itEnd; it++) c.terms.push_back(*it);
	for (; ib != ibEnd; ib++) c.terms.push_back(*ib);
	
	return 0;
}

template<class T>
void sparseMatrix<T>::in() {
	// 矩阵 -> 稀疏矩阵 
	this -> terms.clear();
	
	int _in; cin >> Rows >> Cols;
	for (int i = 1; i <= Rows; i++) {
		for (int j = 1; j <= Cols; j++) {
			cin >> _in;
			if (_in) {
				this -> terms.push_back({i, j, _in});
			}
		}
	}
}

template<class T>
void sparseMatrix<T>::SortbyCol() {
	sort (terms.begin(), terms.end());
}


template<class T>
int sparseMatrix<T>::Mul(sparseMatrix<T> &A, sparseMatrix<T> &Res) {
	if (A.Rows != Cols) return -1;
	
	// 将原本行优先的存储方式改为列优先，之后方便处理出A矩阵（右乘矩阵）列每列有多少个元素
	A.SortbyCol ();	
	
	Res.Rows = Rows;
	Res.Cols = A.Cols;
//	Res.terms.clear();
	Res.terms.resize(0);
	
	vector<matrixTerm<int> >::iterator it = terms.begin();
	vector<matrixTerm<int> >::iterator it_ = A.terms.begin();
	vector<matrixTerm<int> >::iterator End = terms.end();
	vector<matrixTerm<int> >::iterator End_ = A.terms.end();	
	
	// pretreatment for P 行首， Q 列首 
	int P[505], Q[505], p = 0, q = 0;
	int preofP[505], preofQ[505];
	
	memset(P, 0, sizeof P);
	memset(Q, 0, sizeof Q);
	memset(preofP, 0, sizeof preofP);
	memset(preofQ, 0, sizeof preofQ);
	
	for (int pre = 0, cnt = 0; ; it++) {
		if (it == End) {
			P[p] = cnt;
			break;
		}
		if(pre) {
			if (pre != it -> row) {
				P[p] = cnt;
				cnt = 0, p++;
			}
		}
		cnt++;
		pre = it -> row;
	}

	for (int pre = 0, cnt = 0; ; it_++) {
//		cout << it_ -> row << " " << it_ -> col << '\n';
		if (it_ == End_) {
			Q[q] = cnt;
			break;
		}
		if(pre) {
			if (pre != it_ -> col) {
				Q[q] = cnt;
				cnt = 0, q++;
			}
		}
		cnt++;
		pre = it_ -> col;
	}	

	for (int i = 1; i <= p + 1; i++) {
		preofP[i] = preofP[i - 1] + P[i - 1];
	}
	for (int i = 1; i <= q + 1; i++) {
		preofQ[i] = preofQ[i - 1] + Q[i - 1];
	}
	
//	for (int i = 0; i <= q + 1; i++) {
//		cout << preofQ[i] << " ";
//	}
//	cout << "**\n";
	it = terms.begin();
	it_ = A.terms.begin();
	
	// Mul
	
	for (int i = 0; i <= p; i++) {
		for (int j = 0; j <= q; j++) {
			auto iter = it + preofP[i];
			auto iter_ = it_ + preofQ[j];
			// End
			auto ed = it + preofP[i + 1];
			auto ed_ = it_ + preofQ[j + 1];
			
			int acc = 0, curRow = iter -> row, curCol = iter_ -> col;
			while (iter != ed && iter_ != ed_) {
				if (iter -> col == iter_ -> row) {
					acc += iter -> val * iter_ -> val;
					iter++;
					iter_++;
				}
				else if (iter -> col < iter_ -> row) {
					iter++;
				}
				else if (iter -> col > iter_ -> row) {
					iter_++;
				}				
			}
//			cout << acc << "**\n";
//			cout << curRow << " " << curCol << " " << acc << "**\n";
			Res.terms.push_back({curRow, curCol, acc});			
		}
	}
	return 0;
}


template<class T>
int sparseMatrix<T>::Mul_(sparseMatrix<T> &A, sparseMatrix<T> &Res) {
	if (A.Rows != Cols) return -1;
	
	
	Res.Rows = Rows;
	Res.Cols = A.Cols;
	Res.terms.clear();
	
	vector<matrixTerm<int> >::iterator it = terms.begin();
	vector<matrixTerm<int> >::iterator it_;
	vector<matrixTerm<int> >::iterator End = terms.end();
	vector<matrixTerm<int> >::iterator End_ = A.terms.end();	
	
//	int R[505][505];
	int R[250005];
	// (500 - 1) * 500 + 500
	memset(R, 0, sizeof R);
	
	for (;it != End; it++) {
		for (it_ = A.terms.begin(); it_ != End_; it_++) {	// it -> row "++" 
			if (it -> col == it_ -> row) {
				R[(it -> row - 1) * 500 + it_ -> col] += (it -> val) * (it_ -> val); 
			}
			if (it -> col < it_ -> row) break;
		}
	}
	int H = 100;
//	if (terms.size() * A.terms.size() > (int) 5e2) H = 100;
	
	for (int i = 1; i <= H; i++) {
		for (int j = 1; j <= H; j++) {
			int ind = (i - 1) * 500 + j;
			if (R[ind]) {
				Res.terms.push_back({i, j, R[ind]});
			}
		}
	}
	
	return 0;
}

// 运行类
class op {
	public:
		op(int n) { this -> n = n; } 
		void run();
	private:
		int n;
};

void op::run() {
	sparseMatrix<int> P, Q, Res;
	
	for (int i = 1, ch; i <= n; i++) {
		cin >> ch;
		if (ch == 1) {
			P.in();
		}
		else if (ch == 2) {
			// Mul
			cin >> Q;
			if (P.Mul(Q, Res) == -1) {
				cout << -1 << '\n';
				P = Q;
			}
			else P = Res;
		}
		else if (ch == 3) {
			cin >> Q;
			if(P.add(Q, Res) == -1) {
				cout << -1 << '\n';
				P = Q;
			}
			else P = Res;
		}
		else if (ch == 4){
			cout << P;
		}
		else if (ch == 5) {
			// transpose
			P.transpose(Res);
			P = Res; 
		}
	}
}

int main(){
//	freopen("out.txt", "w", stdout);
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	 
	int num; cin >> num;
	op op_(num);
	op_.run();
	return 0;
}

