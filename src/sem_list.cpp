/*
 * sem_SemaphoreList.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#include "kern_sem.h"

SemaphoreList::Elem* SemaphoreList::put(KernelSem* sem) {
	Elem *newEl = new Elem(sem);
	if (head == 0) {
		head = tail = newEl;
	} else {
		tail->next = newEl;
		newEl->prev = (SemaphoreList::Elem*) tail;
		tail = tail->next;
	}
	return newEl;
}

SemaphoreList::Elem* SemaphoreList::unlink_this(Elem *e) {
	if (e == 0)
		return 0;
	Elem *prev = e->prev, *next = e->next;
	if (next == 0)
		tail = prev;
	if (prev == 0)
		head = next;
	if (prev!=0) prev->next = next;
	if (next!=0) next->prev = prev;
	return e;
}

SemaphoreList::SemaphoreList() {
	PCB::lock();
	head = 0;
	tail = 0;
	PCB::unlock();
}

SemaphoreList::~SemaphoreList() {
	PCB::lock();
	SemaphoreList::Elem *t = (SemaphoreList::Elem*)head;
	while (t != 0) {
		head = head->next;
		delete t;
		t = (SemaphoreList::Elem*)head;
	}
	head = tail = 0;
	PCB::unlock();
}

