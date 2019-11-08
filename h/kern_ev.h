/*
 * kern_ev.h
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_KERN_EV_H_
#define KERNEL_H_KERN_EV_H_

#include "define.h"
#include "pcb.h"

class KernelEv {
public:
	KernelEv (IVTNo ivtNo, Event* event);
	~KernelEv();

	void wait();
	void signal();

private:
	IVTNo ivtNo;
	PCB* myCreator;
	int val;
	Event* event;
};




#endif /* KERNEL_H_KERN_EV_H_ */
