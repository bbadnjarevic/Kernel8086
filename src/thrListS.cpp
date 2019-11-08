/*
 * thrThreadListSemS.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#ifndef SOURCE_THRLISTS_CPP_
#define SOURCE_THRLISTS_CPP_

#include "define.h"
#include "pcb.h"

void ThreadListSem::put(PCB* pcb, Time time) {
	Elem *newEl = new Elem(pcb, time);
	if (head == 0) {
		head = tail = newEl;
	} else {
		tail->next = newEl;
		newEl->prev = (ThreadListSem::Elem*)tail;
		tail = tail->next;
	}
}

void ThreadListSem::unlink_this(Elem *e) {
	if (e == 0) return;
	Elem *prev = e->prev,
			*next = e->next;
	if (next == 0) {
		tail = prev;
		tail->next = 0;
	}
	if (prev == 0) {
		head = next;
		head->prev = 0;
	}
	if (prev!=0) prev->next = next;
	if (next!=0) next->prev = prev;
}

PCB* ThreadListSem::get() {
	if (head == 0) return 0;
	Elem *t = (ThreadListSem::Elem*) head;
	PCB *thread = t->pcb;
	if (head == tail) {
		head = 0;
		tail = 0;
	} else {
		head = head->next;
		head->prev = 0;
	}
	delete t;
	return thread;
}





ThreadListSem::ThreadListSem() {
	PCB::lock();
	head = 0;
	tail = 0;
	PCB::unlock();
}

ThreadListSem::~ThreadListSem() {
	PCB::lock();
	ThreadListSem::Elem *t = (ThreadListSem::Elem*)head;
	while (t != 0) {
		head = head->next;
		delete t;
		t = (ThreadListSem::Elem*)head;
	}
	head = tail = 0;
	PCB::unlock();
}






#endif /* SOURCE_THRLISTS_CPP_ */
