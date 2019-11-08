/*
 * userThrd.h
 *
 *  Created on: Aug 29, 2019
 *      Author: OS1
 */

#ifndef H_USERTHRD_H_
#define H_USERTHRD_H_

#include "thread.h"
#include "semaphor.h"

class UserThread : public Thread {
public:
	UserThread() : Thread() {}
	~UserThread() { waitToComplete(); }

	void set(int argc, char** a) {
		this->argc = argc;
		this->argv = a;
	}

	int result() const { return res; }
private:
	int res;
	int argc;
	char** argv;
protected:
	void run() {
		res = userMain(argc, argv);
	}
};




#endif /* H_USERTHRD_H_ */
