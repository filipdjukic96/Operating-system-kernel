/*
 * handler.h
 *
 *  Created on: May 16, 2017
 *      Author: OS1
 */

#ifndef HANDLER_H_
#define HANDLER_H_

#include "thread.h"

void signalHandlerZero();

class SignalHandlersList{
public:

	static void handleSignals();

	struct Member{
			SignalId signal;
			Member* next;
			Member* prev;
			Member(SignalId sign){signal=sign;next=0;prev=0;}
		};

	int numOfMembers;
	Member *first,*last;

	SignalHandlersList();
	~SignalHandlersList();
	void addSignal(SignalId signal);
	void removeSignal(Member* p);
};


#endif /* HANDLER_H_ */
