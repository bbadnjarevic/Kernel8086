/*
 * event.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#include "event.h"
#include "pcb.h"
#include "kern_ev.h"

Event::Event(IVTNo ivtNo) {
	PCB::lock();
	myImpl = new KernelEv(ivtNo, this);
	PCB::unlock();
}

Event::~Event() {
	PCB::lock();
	delete myImpl;
	PCB::unlock();
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
