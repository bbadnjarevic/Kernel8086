/*
 * Thread.cpp
 *
 *  Created on: Jun 29, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "thread.h"
#include "sig_list.h"
#include "sigHandL.h"

void Thread::start() {
	if (myPCB->state == PCB::INIT) {
		PCB::lock();
		myPCB->createStack();
		if (myPCB->id != 1)
			Scheduler::put(myPCB);
		PCB::unlock();
	}
}

void Thread::waitToComplete() {
	if (this->myPCB->state != PCB::FINISHED && this->myPCB->state != PCB::INIT) {
	PCB::lock();
	this->myPCB->waitingThreadsList->put(PCB::running_thread);

	PCB::running_thread->state = PCB::BLOCKED;
	PCB::unlock();
	dispatch();
	}
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
	myPCB = 0;
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running_thread->id;
}

Thread* Thread::getThreadById(ID id) {
	return PCB::threadList->getById(id)->thread;
}

Thread::Thread (StackSize stackSize , Time timeSlice) {
	PCB::lock();
	myPCB = new PCB(this, timeSlice, stackSize);
	PCB::unlock();
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli
#endif
	PCB::explicit_context_switching_request = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif
}


// SIGNALS

void Thread::signal(SignalId signal) {
	if (myPCB->signalsList == 0) myPCB->signalsList = new SignalsList();
	myPCB->signalsList->put(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	myPCB->signalHandlers[signal]->put(handler);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	myPCB->signalHandlers[id]->swap(hand1, hand2);
}

void Thread::unregisterAllHandlers(SignalId id) {
	myPCB->signalHandlers[id]->remove_all();
}

void Thread::blockSignal(SignalId signal) {
	myPCB->blockedSignals = myPCB->blockedSignals | (1 << signal);
}

void Thread::unblockSignal(SignalId signal) {
	myPCB->blockedSignals = myPCB->blockedSignals & (~(1<<signal));
}

void Thread::blockSignalGlobally(SignalId signal) {
	PCB::globalBlockedSignals = PCB::globalBlockedSignals | (1 << signal);
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::globalBlockedSignals = PCB::globalBlockedSignals & (~(1<<signal));
}


void kill_thread() {
	PCB::lock();
	PCB::running_thread->state = PCB::FINISHED;

	PCB *t = 0;
	while ((t = PCB::running_thread->waitingThreadsList->get()) != 0) {
		t->state = PCB::READY;
		Scheduler::put(t);
		t = PCB::running_thread->waitingThreadsList->get();
	}

	if (PCB::running_thread->myParent)
		PCB::running_thread->myParent->signalsList->put(1);
	PCB::running_thread->signalsList->put(2);

	PCB::unlock();
	dispatch();
}



