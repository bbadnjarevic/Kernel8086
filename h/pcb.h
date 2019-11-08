/*
 * pcb.h
 *
 *  Created on: Jun 29, 2019
 *      Author: OS1
 */

#ifndef KERNEL1_H_PCB_H_
#define KERNEL1_H_PCB_H_

#include "define.h"
#include "thrdList.h"
#include "thrListS.h"
#include "kern_sem.h"
#include "sig_list.h"
#include "sigHandL.h"



class PCB {
public:

	PCB (Thread* myThread, Time myTimeSlice, StackSize stackSize);


	static enum State {INIT, READY, RUNNING, BLOCKED, FINISHED};

	// Context Switching
	static void lock();
	static void unlock();

	~PCB();

	static IdleThread* volatile idle_thread;
	static PCB* volatile idle_pcb;
	void createStack();


private:

	static ID idGen;
	ID id;
	State state;
	List *waitingThreadsList;

	// From Thread
	Thread *thread;
	StackSize stackSize;
	Time timeSlice;

	// Registers:
	volatile unsigned sp; // Stack Pointer
	volatile unsigned ss; // Stack Segment
	volatile unsigned bp; // Base Pointer

	unsigned *stack;

	// Helper functions
	static void wrapper();

	volatile int semRet;

	List::Elem* myElemInThreadsList;
	PCB* myParent;


	// GLOBAL VARIABLES:

	static List *threadList;


	// Running thread
	static volatile unsigned running_thread_counter;
	static PCB* volatile running_thread;

	// Context Switching
	static volatile int locked;
	static volatile int explicit_context_switching_request;

	// Signals
	static volatile unsigned globalBlockedSignals;
	volatile unsigned blockedSignals;
	SignalsList *signalsList;
	SignalHandlersList* signalHandlers[numOfSignals];

	// Functions from Thread for main_thread
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);


	// Friend functions
	friend void dispatch();
	friend void interrupt timer(...);
	friend int main(int argc, char* argv[]);
	friend void kill_thread();

	// Friend classes
	friend class List;
	friend class Thread;
	friend class IdleThread;
	friend class KernelSem;
	friend class KernelEv;
	friend class SignalsList;
};



#endif /* KERNEL1_H_PCB_H_ */



