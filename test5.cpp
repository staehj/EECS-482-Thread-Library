#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "thread.h"

using std::cout;
using std::endl;
using std::vector;

// Inputs for testing, for p2
vector<vector<int>> disks =
{ {785, 53, 15, 345, 4, 66, 3, 24, 3},
	{350, 914, 345, 4,},
	{827, 567},
	{302, 230},
	{631, 11} };
int input_commandline_cap = 3;


struct requests {
	int requester;
	int track;
};

enum class requester_thread_state { STOP, GO, LAST, DEAD };

mutex disk_lock;
cv request_queue;
cv service_queue;

int MAX_DISK_QUEUE;
int FILE_ALIVE;
int PREVIOUSLY_SERVICED = 0;
std::vector<requester_thread_state> REQUEST_NEXT;
std::vector<requests> DISK;

void service_thread(void* unused_param) {
	// Service thread is alive until MAX_DISK_QUEUE becomes 0
	while (MAX_DISK_QUEUE != 0) {
		// Wait until DISK is full
		disk_lock.lock();
		while ((int)DISK.size() != MAX_DISK_QUEUE) {
			service_queue.wait(disk_lock);
		}

		// Iterate through DISK to find index of shortest seek time track
		int sstf_index = 0;
		int diff = 1000;
		for (int i = 0; i < (int)DISK.size(); i++) {
			if (abs(DISK[i].track - PREVIOUSLY_SERVICED) < diff) {
				sstf_index = i;
				diff = abs(DISK[i].track - PREVIOUSLY_SERVICED);
			}
		}

		// Change state of thread after servicing, STOP -> GO
		if (REQUEST_NEXT[DISK[sstf_index].requester] == requester_thread_state::STOP) {
			REQUEST_NEXT[DISK[sstf_index].requester] = requester_thread_state::GO;
		}
		// If the request was the last one, LAST -> DEAD and
		// decrement FILE_ALIVE
		else if (REQUEST_NEXT[DISK[sstf_index].requester] == requester_thread_state::LAST) {
			REQUEST_NEXT[DISK[sstf_index].requester] = requester_thread_state::DEAD;
			FILE_ALIVE--;
		}
		// If the state is GO or DEAD, something is going wrong
		else {
			assert(false);
		}

		// Pop SSTF out of DISK
		PREVIOUSLY_SERVICED = DISK[sstf_index].track;
		DISK.erase(DISK.begin() + sstf_index);

		// Modify MAX_DISK_QUEUE before going to the start of while loop
		MAX_DISK_QUEUE = std::min(MAX_DISK_QUEUE, FILE_ALIVE);

		request_queue.broadcast();
		disk_lock.unlock();
	}
}

void request_thread(void* requester_in) {
	// Decompose input parameter
	intptr_t requester_intptr = (intptr_t)requester_in;
	int requester = (int)requester_intptr;

	// Requester thread is alive until all tracks in a file are requested
	disk_lock.lock();
	for (auto& track : disks[requester]) {
		// Wait when the previously requested track is not serviced yet OR
		// disk is full
		while (REQUEST_NEXT[requester] == requester_thread_state::STOP ||
			(int)DISK.size() == MAX_DISK_QUEUE) {
			request_queue.wait(disk_lock);
		}

		// Initially, or when the previously requested track is serviced,
		// Push the request currently read from vector
		requests new_request = { requester, track };
		DISK.push_back(new_request);

		// After requesting, set enum class vector element to STOP
		REQUEST_NEXT[requester] = requester_thread_state::STOP;
		service_queue.signal();
	}

	// After reading every track in one file, change its state to LAST
	REQUEST_NEXT[requester] = requester_thread_state::LAST;
	disk_lock.unlock();
}

void initial_thread(void* unused_param) {

	thread servicer_t((thread_startfunc_t)service_thread, (void*)100);

	// Make of vector of thread states - for each requester
	for (int i = 0; i < FILE_ALIVE; i++) {
		REQUEST_NEXT.push_back(requester_thread_state::GO);
	}

	// For each input in commandline, initiate request thread
	for (int i = 0; i < FILE_ALIVE; i++) {
		intptr_t intptr = i;
		thread requester_t((thread_startfunc_t)request_thread, (void*)intptr);
	}
}

int main() {
	// If file_count is smaller than given capacity, MAX_DISK_QUEUE = file_count
	int commandline_cap = input_commandline_cap;
	FILE_ALIVE = (int)disks.size();
	MAX_DISK_QUEUE = std::min(commandline_cap, FILE_ALIVE);
	cpu::boot(1, (thread_startfunc_t)initial_thread, (void*)100, false, false, 0);
}