#ifndef _idleThr_h_
#define _idleThr_h_
#include "define.h"
#include "thread.h"

class IdleThread : public Thread {
private:
	int finished;
public :
	IdleThread():Thread(defaultStackSize, 1) { finished = 0; }
	~IdleThread() {
		finished = 1;
		waitToComplete();
	}

protected:
	void run();
};

#endif
