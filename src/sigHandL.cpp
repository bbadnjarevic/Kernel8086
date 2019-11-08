/*
 * sigHandL.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */


#include "sigHandL.h"
#include "pcb.h"

SignalHandlersList::SignalHandlersList() {
	head = 0;
	tail = 0;
}

SignalHandlersList::~SignalHandlersList() {
	this->remove_all();
}

void SignalHandlersList::remove_all() {
	PCB::lock();

	Elem *t = 0;
	while ((t = (Elem*)head)) {
		head = head->next;
		delete t;
	}

	head = 0;
	tail = 0;

	PCB::unlock();
}

void SignalHandlersList::put(SignalHandler signalHandler) {

	Elem *newEl = new Elem(signalHandler);
	if (head == 0) {
		head = tail = newEl;
	} else {
		tail->next = newEl;
		newEl->prev = (SignalHandlersList::Elem*) tail;
		tail = tail->next;
	}

}

void SignalHandlersList::swap(SignalHandler s1, SignalHandler s2) {

	PCB::lock();
	if (head == 0) {
		PCB::unlock();
		return;
	}
	Elem *t1 = 0, *t2 = 0, *t = (Elem*)head;

	while (t && (t1==0 || t2==0)) {
		if (t->signalHandler == s1 || t->signalHandler == s2) {
			if (t1 == 0) t1 = t;
			else t2 = t;
		}
		t = t->next;
	}

	if (t1==0 || t2==0) {
		PCB::unlock();
		return;
	}

	Elem *t1_next = t1->next, *t1_prev = t1->prev,
			*t2_next = t2->next, *t2_prev = t2->prev;

	if (t1_next != t2) {
		t1->next = t2_next;
		t1->prev = t2_prev;
		if (t2_next) t2_next->prev = t1;
		if (t2_prev) t2_prev->next = t1;

		t2->next = t1_next;
		t2->prev = t1_prev;
		if (t1_next) t1_next->prev = t2;
		if (t1_prev) t1_prev->next = t1;
	} else {
		t1->prev = t2;
		t1->next = t2_next;
		t2->prev = t1_prev;
		t2->next = t1;
		if (t2_next) t2_next->prev = t1;
		if (t1_prev) t1_prev->next = t2;
	}
	if (head == t1) head = t2;
	if (tail == t2) tail = t1;

	PCB::unlock();

}

void SignalHandlersList::callAll() {
	Elem *t = (Elem*)head;
	while (t) {
		t->signalHandler();
		t = t->next;
	}
}


void SignalHandlersList::clone(SignalHandlersList* dst, SignalHandlersList* src) {
	SignalHandlersList::Elem *t = (SignalHandlersList::Elem*)src->head;
	while (t) {
		dst->put(t->signalHandler);
		t = t->next;
	}
}












