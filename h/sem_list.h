/*
 * sem_list.h
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_SEM_LIST_H_
#define KERNEL_H_SEM_LIST_H_

#include "define.h"

class SemaphoreList {
public:

	struct Elem {
			KernelSem* sem;
			Elem* next;
			Elem* prev;
			Elem(KernelSem *t){
				sem = t;
				next = 0;
				prev = 0;
			}
	};

	Elem* put(KernelSem* sem);
	Elem* unlink_this(Elem *e);



	SemaphoreList();
	~SemaphoreList();

private:

	volatile Elem* head, *tail;

	friend class KernelSem;

};



#endif /* KERNEL_H_SEM_LIST_H_ */
