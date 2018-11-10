/*
 * event.h
 *
 *  Created on: May 6, 2017
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "ivtentry.h"

#define PREPAREENTRY(ivtNo, callOld) \
void interrupt inter##ivtNo(...); \
IVTEntry newEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) { \
	if (callOld == 1 && newEntry##ivtNo.oldInterruptRoutine != 0) \
		(*newEntry##ivtNo.oldInterruptRoutine)(); \
	if (newEntry##ivtNo.myEvent)\
		newEntry##ivtNo.myEvent->signal(); \
	dispatch(); \
}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};


#endif /* EVENT_H_ */
