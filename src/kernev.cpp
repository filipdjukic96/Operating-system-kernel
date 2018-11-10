/*
 * kernev.cpp
 *
 *  Created on: May 6, 2017
 *      Author: OS1
 */

#include "kernev.h"
#include "schedule.h"
#include "pcb.h"
#include "ivtentry.h"
#include <stdlib.h>

KernelEv::~KernelEv(){
	if (IVTEntry::ivTable[ivtNumber]!=0)
		IVTEntry::ivTable[ivtNumber]->myEvent=0;
	signal();
}

KernelEv::KernelEv(IVTNo ivtNo):val(0),myPCB(PCB::runningThread),ivtNumber(ivtNo){
	if (IVTEntry::ivTable[ivtNumber]!=0)
		IVTEntry::ivTable[ivtNumber]->myEvent=this;
	else
		exit(1);
}

void KernelEv::wait(){

	if(myPCB!=PCB::runningThread) return;

	if(val==0){
		myPCB->threadState=BLOCKED;
		val--;
		dispatch();
	}else if(val==1 ){
		val--;
	}
}

void KernelEv::signal(){

	if(val==1) return;

	if(val==0)
		val++;
	else{
		myPCB->threadState=READY;
		Scheduler::put(myPCB);
		val++;
	}

}
