/*
 * thrThreadListSemS.h
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_THRLISTS_H_
#define KERNEL_H_THRLISTS_H_


class ThreadListSem {
public:
	struct Elem {
				PCB* pcb;
				Time timeToWait;
				Elem* next;
				Elem* prev;
				Elem(PCB *t, Time time){
					pcb = t;
					timeToWait = time;
					next = 0;
					prev = 0;
				}
		};

	void put(PCB* pcb, Time time);
	void unlink_this(Elem *e);
	PCB* get();

	ThreadListSem();
	~ThreadListSem();



private:

	Elem volatile *head;
	Elem volatile *tail;


	friend class PCB;
	friend class Thread;
	friend class KernelSem;

};



#endif /* KERNEL_H_THRLISTS_H_ */
