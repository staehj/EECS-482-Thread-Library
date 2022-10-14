#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "thread.h"

using std::cout;
using std::endl;

void func(void *a) {
	cout << (char*)a << " pre-lock" << endl;
	thread::yield();
	cout << (char*)a << " critical" << endl;
	cout << (char*)a << " done" << endl;
}

void initial_thread(void* unused ) {
	cout << "before thread 1\n";
	thread tA(func, (void*)"A");
	cout << "before thread 2\n";
	thread tB(func, (void*)"B");
	cout << "after\n";
}

int main(void) {
	cout << "starting\n";
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}