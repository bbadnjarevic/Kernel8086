/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "kern_ev.h"

IVTEntry* IVTEntry::enteries[numOfEnetries] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, interruptPointer iPointer) {
	PCB::lock();
	if (IVTEntry::enteries[ivtNo]) {
		PCB::unlock();
		return;
	}
#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, iPointer);
#endif
	this->newRoutine = iPointer;
	this->ivtNo = ivtNo;
	IVTEntry::enteries[ivtNo] = this;
	PCB::unlock();
}

void IVTEntry::signal() {
	if (!myKernelEv) return;
	myKernelEv->signal();
	dispatch();
}

IVTEntry* IVTEntry::get(IVTNo ivtNo) {
	return IVTEntry::enteries[ivtNo];
}


void IVTEntry::callOld() {
	this->oldRoutine();
}


IVTEntry::~IVTEntry() {
	PCB::lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
	IVTEntry::enteries[ivtNo] = 0;
	oldRoutine = 0;
	newRoutine = 0;
	PCB::unlock();
}
