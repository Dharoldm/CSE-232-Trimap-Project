#include<sstream>
using std::ostringstream;
#include<string>
using std::string;
#include <iostream>
using std::cout; using std::endl;
#include "proj11_trimap.h"
#include <cassert>
#include <assert.h>



int main(){
	TriMap <long, long>m(3,50);
	m.insert(2,20);
	m.insert(1, 10);
	m.insert(4,40);
	m.insert(5,30);
	m.insert(12,24);
	m.insert(345,34);
	m.insert(-345,34);
	cout << m << endl;
	bool result = m.remove(-345);
	size_t sz = m.size();
	cout << result<< ":" << sz << endl;
	cout << "before m" << endl;
	cout << m << endl;
	m.insert(6,1);
	cout << m << endl;

	
}