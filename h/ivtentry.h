/*
 * ivtentry.h
 *
 *  Created on: May 9, 2017
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "kernev.h"

const int numOfInterruptEntries=256;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
public:
	static IVTEntry* ivTable[numOfInterruptEntries];
	static IVTEntry* getEntry(IVTNo ivtNumber);
	IVTEntry(IVTNo ivtNumber,pInterrupt odlRoutine);
	~IVTEntry();


	pInterrupt oldInterruptRoutine;
	IVTNo ivtNumber;
	KernelEv* myEvent;

};



#endif /* IVTENTRY_H_ */
