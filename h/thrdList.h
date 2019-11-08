/*
 * List.h
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */

#ifndef KERNEL1_H_LIST_H_
#define KERNEL1_H_LIST_H_

#include "define.h"
#include "thread.h"

class List {
public:

	struct Elem {
			PCB* pcb;
			Elem* next;
			Elem* prev;
			Elem(PCB *t){
				pcb = t;
				next = 0;
				prev = 0;
			}
	};

	Elem* put(PCB* pcb);
	Elem* unlink(ID threadID);
	void unlink_this(Elem* e);
	PCB* getById(ID id);
	PCB* get();

	List();
	~List();

private:

	volatile Elem* head, *tail;


	friend class PCB;
	friend class Thread;

};



#endif /* KERNEL1_H_LIST_H_ */
