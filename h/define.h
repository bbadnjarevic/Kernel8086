#ifndef _define_h_
#define _define_h_

#include "SCHEDULE.h"
#include <DOS.H>

// Classes
class PCB;
class Thread;
class IdleThread;
class List;
class Semaphore;
class KernelSem;
class KernelEv;
class Event;
class IVTEntry;

// Data types
#ifndef _thread_h_
typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef int ID;
typedef void (*SignalHandler)();
typedef unsigned SignalId;
#endif

#ifndef _semaphor_h_
typedef unsigned int Time;
#endif

#ifndef _event_h_
typedef unsigned char IVTNo;
#endif

// Functions
void tick();
void interrupt timer(...);
int userMain(int argc, char* argv[]);

// Interrupt
typedef void interrupt (*interruptPointer)(...);
extern interruptPointer oldRoutine;

#define PREPAREENTRY(ivtNo, callOldFlag) \
void interrupt inter##ivtNo(...); \
IVTEntry ivtEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) { \
	ivtEntry##ivtNo.signal(); \
	if(callOldFlag) \
		ivtEntry##ivtNo.callOld(); \
}

// Signals
void kill_thread();

// Constants
const StackSize maxStackSize = 65536;
const int numOfEnetries = 256;
const int numOfSignals = 16;


#endif
