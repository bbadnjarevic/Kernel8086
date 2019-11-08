/*
 * sig_list.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#include "sig_list.h"
#include "pcb.h"

SignalsList::SignalsList() {
	head = 0;
	tail = 0;
}

SignalsList::~SignalsList() {
	PCB::lock();

	Elem *t = 0;
	while (head) {
		t = (Elem*) head;
		head = head->next;
		delete t;
	}

	head = 0;
	tail = 0;

	PCB::unlock();
}


void SignalsList::put(SignalId signalId) {
	PCB::lock();

	Elem *newEl = new Elem(signalId);
	if (head == 0) {
		head = tail = newEl;
	} else {
		tail->next = newEl;
		newEl->prev = (SignalsList::Elem*)tail;
		tail = tail->next;
	}

	PCB::unlock();
}

void SignalsList::remove_this(Elem *e) {
	if (e == 0)
		return;
	Elem *prev = e->prev, *next = e->next;
	if (next == 0) {
		tail = prev;
		tail->next = 0;
	}
	if (prev == 0) {
		head = next;
		head->prev = 0;
	}
	if (prev != 0)
		prev->next = next;
	if (next != 0)
		next->prev = prev;
	delete e;
}

void SignalsList::process() {
#ifndef BCC_BLOCK_IGNORE
	asm sti
#endif

	unsigned mask1 = PCB::globalBlockedSignals,
			mask2 = PCB::running_thread->blockedSignals;
	Elem* t = (Elem*)head, *tt = 0;
	while (t) {
		if (!(mask1&(1<<t->signalId)) && !(mask2&(1<<t->signalId))) {
			if (PCB::running_thread->signalHandlers[t->signalId])
				PCB::running_thread->signalHandlers[t->signalId]->callAll();
			tt = t;
			t = t->next;
			remove_this(tt);
		} else t = t->next;
	}


#ifndef BCC_BLOCK_IGNORE
	asm cli
#endif
}




