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
cv wait_queue;
bool condition = false;

void funcB_1(void* param) {
	while (true);
	m.lock();
	condition = true;
	m.unlock();
}

void funcA(void* param) {
	cout << "Thread A" << endl;
	while (!condition) {
		wait_queue.wait(m);
	}
	cout << "condition is true now" << endl;
}

void funcB(void* param) {
	cout << "Thread B" << endl;
	thread tB_1(funcB_1, (void*)"B");
	tB_1.join();
	cout << "join for B child has finished" << endl;
	m.lock();
	condition = true;
	wait_queue.signal();
	m.unlock();
	thread::yield();
}

void initial_thread(void* unused ) {
	thread tA(funcA, (void*)"A");
	thread tB(funcB, (void*)"B");
}

int main(void) {
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}
