/*
 * sig_list.h
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#ifndef H_SIG_LIST_H_
#define H_SIG_LIST_H_

#include "define.h"

class SignalsList {
private:

	struct Elem {
		Elem *prev, *next;
		SignalId signalId;
		Elem(SignalId s): signalId(s), prev(0), next(0) {}
	};

	Elem volatile *head;
	Elem volatile *tail;

public:

	SignalsList();
	~SignalsList();

	void put(SignalId signalId);
	void remove_this(Elem *e);
	Elem* get_first();

	void process();

	friend class PCB;

};




#endif /* H_SIG_LIST_H_ */
