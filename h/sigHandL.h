/*
 * sigHandL.h
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#ifndef H_SIGHANDL_H_
#define H_SIGHANDL_H_

#include "define.h"

class SignalHandlersList {
private:
	struct Elem {
		Elem *prev, *next;
		SignalHandler signalHandler;
		Elem(SignalHandler s): signalHandler(s), prev(0), next(0) {}
	};

	Elem volatile *head;
	Elem volatile *tail;

public:
	SignalHandlersList();
	~SignalHandlersList();

	void put(SignalHandler signalHandler);
	void swap(SignalHandler s1, SignalHandler s2);
	void remove_all();

	void callAll();

	static void clone(SignalHandlersList* dst, SignalHandlersList* src);
};



#endif /* H_SIGHANDL_H_ */
