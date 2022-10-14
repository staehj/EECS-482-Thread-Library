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

void initial_thread(void* unused ) {
	thread tA(func, (void*)"A");
	thread tB(func, (void*)"B");
	thread tC(func, (void*)"C");
	thread tD(func, (void*)"D");
	thread tE(func, (void*)"E");
	thread tF(func, (void*)"F");
	thread tG(func, (void*)"G");
}

int main(void) {
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}