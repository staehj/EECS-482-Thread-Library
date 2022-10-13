#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "thread.h"

void A(void*) {
	std::cout << "A yielding to B\n";
	thread::yield();
	std::cout << "A finishing\n";
}

void B(void*) {
	thread A_thread((thread_startfunc_t)A, (void*)100);
	std::cout << "B wait on A\n";
	A_thread.join();
	std::cout << "B wait on A again\n";
	A_thread.join();
	std::cout << "B finishing\n";
}

int main(void) {
	thread B_thread((thread_startfunc_t)B, (void*)100);
}