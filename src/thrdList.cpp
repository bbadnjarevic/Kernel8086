/*
 * List.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */

#include "pcb.h"

List::Elem* List::put(PCB* pcb) {
	Elem *newEl = new Elem(pcb);
	if (head == 0) {
		head = tail = newEl;
	} else {
		tail->next = newEl;
		newEl->prev = (List::Elem*)tail;
		tail = tail->next;
	}
	return newEl;
}

List::Elem* List::unlink(ID threadID) {
	Elem *t = (List::Elem*)head;
	while (t != 0 && t->pcb->id!= threadID) {
		t = t->next;
	}
	if (t->pcb->id == threadID) { // Found thread

		if (head == tail) {
			head = 0;
			tail = 0;
		} else if (t == head) {
			head = head->next;
			head->prev = 0;
		} else if (t == tail) {
			tail = tail->prev;
			tail->next = 0;
		}

		return t;
	}
	return 0;

}

List::List() {
	head = 0;
	tail = 0;
}

PCB* List::getById(ID id) {
	Elem *t =(List::Elem*)head;
	while (t != 0 && t->pcb->id != id) t = t->next;
	if (t->pcb->id == id) {
		return t->pcb;
	} else return 0;
}

PCB* List::get() {
	if (head == 0) return 0;
	Elem *t = (List::Elem*) head;
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

List::~List() {
	PCB::lock();
	List::Elem *t = (List::Elem*)head;
	while (t != 0) {
		head = head->next;
		delete t;
		t = (List::Elem*)head;
	}
	head = tail = 0;
	PCB::unlock();
}

void List::unlink_this(Elem *e) {
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
}


