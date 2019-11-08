/*
 * userMain.cpp
 *
 *  Created on: Aug 13, 2019
 *      Author: OS1
 */

#include "define.h"
#include "idleThr.h"
#include "pcb.h"
#include "userThrd.h"
#include <iostream.h>

interruptPointer oldRoutine = 0;



int main(int argc, char* argv[]) {
	PCB::lock();
	cout << "Main Started!\n";
	PCB::unlock();

	int ret = 0;
	PCB::idle_thread->start();

	UserThread userThread;
	userThread.set(argc, argv);
	userThread.registerHandler(0, &kill_thread);

	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);
	setvect(0x08, timer);
#endif

	userThread.start();
	userThread.waitToComplete();
	PCB::lock();
	cout << "userThread Ended!\n";
	PCB::unlock();
	ret = userThread.result();

#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldRoutine);
#endif


	delete KernelSem::semaphoreList;
	delete PCB::idle_thread;
	delete PCB::running_thread;
	delete PCB::threadList;

	PCB::lock();
	cout << "Main Ended!";
	PCB::unlock();
	return ret;
}


