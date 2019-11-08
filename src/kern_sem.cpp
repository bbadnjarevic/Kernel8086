/*
 * kern_sem.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#include "kern_sem.h"

// Static members:
SemaphoreList* volatile KernelSem::semaphoreList;

void KernelSem::tick() {
	PCB::lock();
	if (semaphoreList == 0) {
		PCB::unlock();
		return;
	}

	SemaphoreList::Elem *t = (SemaphoreList::Elem*)semaphoreList->head;
	while (t != 0) {
		t->sem->sem_tick();
		t = t->next;
	}
	PCB::unlock();
}

KernelSem::KernelSem(Semaphore *mySemaphore, int init) {
	this->mySempahore = mySemaphore;
	this->val = init;
	blockedThreadsList = new ThreadListSem();
	myElem = 0;
	if (semaphoreList == 0) semaphoreList = new SemaphoreList();
	myElem = semaphoreList->put(this);
}

void KernelSem::sem_tick() {
	PCB::lock();
	ThreadListSem::Elem *t = (ThreadListSem::Elem*)blockedThreadsList->head;
	while (t!=0) {

		if (t->timeToWait > 0) t->timeToWait--;
		if (t->timeToWait == 0) {
			ThreadListSem::Elem *tt = t;
			val++;
			t = t->next;
			tt->pcb->semRet = 0;
			tt->pcb->state = PCB::READY;
			Scheduler::put(tt->pcb);
			blockedThreadsList->unlink_this(tt);
			delete tt;
		} else
			t = t->next;
	}
	PCB::unlock();
}



int KernelSem::wait(Time maxTimeToWait) {
	PCB::lock();
	val--;
	if (val < 0) {
		if (maxTimeToWait == 0) maxTimeToWait = -1;
		blockedThreadsList->put((PCB*)PCB::running_thread, maxTimeToWait);
		PCB::running_thread->state = PCB::BLOCKED;
		PCB::unlock();
		dispatch();
		return PCB::running_thread->semRet?1:((PCB::running_thread->semRet = 1)-1);
	} else {
		PCB::unlock();
		return 1;
	}
}

int KernelSem::signal(int n) {
	PCB::lock();
	if (n < 0) {
		PCB::unlock();
		return n;
	} else if (n == 0) {
		if (val++ < 0) {
			PCB* t = blockedThreadsList->get();
			if (t->state == PCB::BLOCKED) {
				t->state = PCB::READY;
				Scheduler::put(t);
			}
			PCB::unlock();
			return 0;
		} else {
			PCB::unlock();
			return 0;
		}
	} else { // n > 0
		int ret = 0;
		if (val < 0)
			ret = n > (-1) * val ? (-1) * val : n;
		val += n;
		for (int i = 0; i < ret; i++) {
			PCB* t = blockedThreadsList->get();
			t->state = PCB::READY;
			Scheduler::put(t);
		}
		PCB::unlock();
		return ret;
	}
}

int KernelSem::value() const {
	return this->val;
}

KernelSem::~KernelSem() {
	PCB::lock();
	semaphoreList->unlink_this(myElem);
	delete myElem;

	while (val < 0) {
		PCB* t = blockedThreadsList->get();
		if (t == 0) break;
		if (t->state == PCB::BLOCKED) {
			t->state = PCB::READY;
			Scheduler::put(t);
		}
		val++;
	}
	delete blockedThreadsList;
	myElem = 0;
	blockedThreadsList = 0;
	PCB::unlock();

}
