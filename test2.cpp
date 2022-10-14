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
}

int main(void) {
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}