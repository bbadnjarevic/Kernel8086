/*
 * kern_ev.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#include "kern_ev.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo, Event *event) {
	this->event = event;
	this->ivtNo = ivtNo;
	this->val = 0;
	IVTEntry::enteries[ivtNo]->myKernelEv = this;
	this->myCreator = PCB::running_thread;
}

void KernelEv::wait() {
	PCB::lock();
	if (myCreator != PCB::running_thread) return;
	if (!val) {
		PCB::running_thread->state = PCB::BLOCKED;
		PCB::unlock();
		dispatch();
	} else {
		val = 0;
		PCB::unlock();
	}
}

void KernelEv::signal() {
	PCB::lock();

	if (myCreator->state != PCB::BLOCKED && val)
		val = 1;
	else {
		myCreator->state = PCB::READY;
		Scheduler::put(myCreator);
	}


	PCB::unlock();
}

KernelEv::~KernelEv() {
	PCB::lock();
	IVTEntry::enteries[ivtNo] = 0;
	PCB::unlock();
}

