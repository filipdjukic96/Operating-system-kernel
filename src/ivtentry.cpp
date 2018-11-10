/*
 * ivtentry.cpp
 *
 *  Created on: May 9, 2017
 *      Author: OS1
 */
#include <dos.h>
#include "ivtentry.h"
#include "pcb.h"
IVTEntry* IVTEntry::ivTable[numOfInterruptEntries]={0};

IVTEntry::IVTEntry(IVTNo ivtNo,pInterrupt newRoutine):myEvent(0) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ivtNumber=ivtNo;
	ivTable[ivtNumber]=this;
#ifndef BCC_BLOCK_IGNORE
	oldInterruptRoutine=getvect(ivtNo);
	setvect(ivtNo,newRoutine);
	unlock
#endif

}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(ivtNumber,oldInterruptRoutine);
#endif
	ivTable[ivtNumber]=0;
	myEvent=0;
	if (oldInterruptRoutine)
		(*oldInterruptRoutine)();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	IVTEntry* ivtEntry=ivTable[ivtNo];
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ivtEntry;
}

