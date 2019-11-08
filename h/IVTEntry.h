/*
 * IVTEntry.h
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_IVTENTRY_H_
#define KERNEL_H_IVTENTRY_H_

#include "define.h"
#include "pcb.h"
#include <DOS.H>

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, interruptPointer iPointer);
	~IVTEntry();

	void callOld();
	void signal();

	static IVTEntry* get(IVTNo ivtNo);

private:
	interruptPointer volatile oldRoutine;
	interruptPointer volatile newRoutine;
	IVTNo ivtNo;
	KernelEv* myKernelEv;

	static IVTEntry* enteries[numOfEnetries];

	friend class KernelEv;
};



#endif /* KERNEL_H_IVTENTRY_H_ */
