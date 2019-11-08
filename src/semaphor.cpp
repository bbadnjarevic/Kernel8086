/*
 * semaphor.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "kern_sem.h"


Semaphore::Semaphore(int init) {
	PCB::lock();
	myImpl = new KernelSem(this, init);
	PCB::unlock();
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->value();
}

Semaphore::~Semaphore() {
	PCB::lock();
	delete myImpl;
	PCB::unlock();
}

