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
void boot_func(void *) {
	thread tA(func, (void*)"A");
	thread tB(func, (void*)"B");
}