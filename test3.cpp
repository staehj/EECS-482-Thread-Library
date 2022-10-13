#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "thread.h"

using std::cout;
using std::endl;

mutex m;

void func(void *a) {
	cout << (char*)a << " pre-lock" << endl;
	m.lock();
	thread::yield();
	cout << (char*)a << " critical" << endl;
	m.unlock();
	cout << (char*)a << " done" << endl;
}
int main(void) {
	thread tA(func, (void*)"A");
	thread tB(func, (void*)"B");
	thread tC(func, (void*)"C");
	thread tD(func, (void*)"D");
	thread tE(func, (void*)"E");
	thread tF(func, (void*)"F");
	thread tG(func, (void*)"G");
}