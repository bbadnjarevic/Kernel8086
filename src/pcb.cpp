/*
 * pcb.cpp
 *
 *  Created on: Jun 29, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "Thread.h"
#include "idleThr.h"


// Global fields:

ID PCB::idGen = 0;
volatile int PCB::locked = 0;
volatile int PCB::explicit_context_switching_request = 0;
List *PCB::threadList = new List();
PCB* volatile PCB::running_thread = new PCB(0, 0, 0); // ThreadID = 0;
volatile unsigned PCB::running_thread_counter = -1;
IdleThread* volatile PCB::idle_thread = new IdleThread(); // ThreadID = 1
PCB* volatile PCB::idle_pcb = PCB::idle_thread->myPCB;

// Signals
volatile unsigned PCB::globalBlockedSignals = 0;



PCB::PCB(Thread* myThread, Time myTimeSlice, StackSize stackSize) {
	PCB::lock();
	thread = myThread;
	timeSlice = myTimeSlice;
	this->stackSize = stackSize/sizeof(unsigned);
	if (maxStackSize < this->stackSize)
		this->stackSize = maxStackSize;
	id = idGen++;
	if (idGen == 3) idGen = 10;
	state = INIT;
	if (!threadList) threadList = new List();
	myElemInThreadsList = threadList->put(this);
	waitingThreadsList = new List();
	this->stack = 0;
	if (id == 0) {
		state = RUNNING;
	}
	semRet = 1;

	for (int i = 0; i < numOfSignals; i++)
		signalHandlers[i] = new SignalHandlersList();
	signalsList = new SignalsList();

	if (id != 0 && id != 1) {
		blockedSignals = PCB::running_thread->blockedSignals;
		myParent = (PCB*)PCB::running_thread;

		for (int i = 0; i < numOfSignals; i++)
			SignalHandlersList::clone(signalHandlers[i], myParent->signalHandlers[i]);

	} else {
		myParent = 0;
		blockedSignals = 0;
	}


	PCB::unlock();
}

// Context Switching
void PCB::lock() {
	locked++;
}
void PCB::unlock() {
	locked--;
	if (locked == 0 && PCB::explicit_context_switching_request) dispatch();
}


void PCB::wrapper() {
	PCB::running_thread->thread->run();
	PCB::lock();
	PCB::running_thread->state = FINISHED;

	PCB *t = 0;
	while ((t = running_thread->waitingThreadsList->get()) != 0) {
		t->state = READY;
		Scheduler::put(t);
		t = running_thread->waitingThreadsList->get();
	}

	if (PCB::running_thread->myParent)
		PCB::running_thread->myParent->signalsList->put(1);
	PCB::running_thread->signalsList->put(2);

	PCB::unlock();
	dispatch();
}

PCB::~PCB() {
	PCB::lock();
	if (myElemInThreadsList) {
		threadList->unlink_this(myElemInThreadsList);
		delete myElemInThreadsList;
	}
	if (stack) delete stack;
	if (waitingThreadsList) delete waitingThreadsList;
	if (signalsList) delete signalsList;
	for (int i = 0; i < numOfSignals; i++)
		if (signalHandlers[i])  {
			delete signalHandlers[i];
			signalHandlers[i] = 0;
		}
	stack = 0;
	waitingThreadsList = 0;
	myElemInThreadsList = 0;
	signalsList = 0;

	PCB::unlock();
}

void PCB::createStack() {
	stack = new unsigned[stackSize];
	// Initialize stack:
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-1] = FP_SEG(thread);
		stack[stackSize-2] = FP_OFF(thread);
	#endif
		stack[stackSize-5] = 0x200;	// PSW
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-6] = FP_SEG(PCB::wrapper);
		stack[stackSize-7] = FP_OFF(PCB::wrapper);
		ss = FP_SEG(stack + stackSize - 16);
		sp = FP_OFF(stack + stackSize - 16);
	#endif
	bp = sp;
	state=READY;
}


// Timer:
volatile unsigned tsp = 0;
volatile unsigned tss = 0;
volatile unsigned tbp = 0;

void interrupt timer(...) {

	if (!PCB::explicit_context_switching_request) {
			oldRoutine();
			tick();
			KernelSem::tick();
	}


	if (!PCB::explicit_context_switching_request && PCB::running_thread_counter > 0)
		PCB::running_thread_counter--;


	if (PCB::running_thread_counter == 0 || PCB::explicit_context_switching_request) {
		if (!PCB::locked) {

			if (PCB::running_thread->signalsList) {
				PCB::locked++;
				PCB::running_thread->signalsList->process();
				PCB::locked--;
			}
			PCB::explicit_context_switching_request = 0;


#ifndef BCC_BLOCK_IGNORE
			asm {
				mov word ptr tbp, bp
				mov word ptr tsp, sp
				mov word ptr tss, ss
			}
#endif

			PCB::running_thread->bp = tbp;
			PCB::running_thread->sp = tsp;
			PCB::running_thread->ss = tss;


			if (PCB::running_thread->state == PCB::RUNNING) {
				PCB::running_thread->state = PCB::READY;
				if (PCB::running_thread->id != 1)
					Scheduler::put((PCB*)PCB::running_thread);
			}


			PCB::running_thread = Scheduler::get();
			if (PCB::running_thread == 0) {
				PCB::running_thread = PCB::idle_pcb;
			}

			PCB::running_thread->state = PCB::RUNNING;

			tsp = PCB::running_thread->sp;
			tss = PCB::running_thread->ss;
			tbp = PCB::running_thread->bp;

			PCB::running_thread_counter = PCB::running_thread->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, word ptr tsp
				mov ss, word ptr tss
				mov bp, word ptr tbp
			}
#endif

		} else
			PCB::explicit_context_switching_request = 1;
	}



}












