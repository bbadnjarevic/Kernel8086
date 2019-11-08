/*
 * kern_sem.h
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_KERN_SEM_H_
#define KERNEL_H_KERN_SEM_H_

#include "define.h"
#include "pcb.h"
#include "sem_list.h"
#include "thrListS.h"

class KernelSem {
public:
	KernelSem(Semaphore *mySemaphore, int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);
	int value() const;

	static void tick();
	void sem_tick();

private:
	int volatile val;
	Semaphore *mySempahore;
	SemaphoreList::Elem* myElem;

	ThreadListSem *blockedThreadsList;

	// GLOBAL VARIABLES:
	static SemaphoreList* volatile semaphoreList;


	friend int main(int argc, char* argv[]);

};




#endif /* KERNEL_H_KERN_SEM_H_ */
