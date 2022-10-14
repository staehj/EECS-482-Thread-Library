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
	m.lock();
	condition = false;
	m.unlock();
}

void funcB_2(void* param) {
	m.lock();
	condition = false;
	m.unlock();
}

void funcB_3(void* param) {
	m.lock();
	condition = false;
	m.unlock();
}

void funcB_4(void* param) {
	thread::yield();
	m.lock();
	condition = false;
	m.unlock();
}

void funcB_5(void* param) {
	thread::yield();
	m.lock();
	condition = false;
	m.unlock();
	m.unlock();
}

void funcA(void* param) {
	cout << "Thread A" << endl;
	m.lock();
	while (!condition) {
		wait_queue.wait(m);
	}
	m.unlock();
	cout << "condition is true now" << endl;
}

void funcB(void* param) {
	cout << "Thread B" << endl;
	thread tB_1(funcB_1, (void*)"B");
	thread tB_2(funcB_2, (void*)"B");
	thread tB_3(funcB_3, (void*)"B");
	thread tB_4(funcB_4, (void*)"B");
	thread tB_5(funcB_5, (void*)"B");
	cout << "join for B child has finished" << endl;
	thread::yield();
	thread::yield();
	thread::yield();
	m.lock();
	condition = true;
	wait_queue.signal();
	m.unlock();
}

void initial_thread(void* unused ) {
	thread tA(funcA, (void*)"A");
	thread tB(funcB, (void*)"B");
}

int main(void) {
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}
