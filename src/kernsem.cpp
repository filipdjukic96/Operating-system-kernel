/*
 * kernsem.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: OS1
 */
#include "kernsem.h"
#include "schedule.h"
#include "pcb.h"

KernelSem* KernelSem::kernelSemHead = 0;


KernelSem::KernelSem(int initValue){
	if(initValue<0) val=0;
	else val=initValue;

	blocked=new List();
	sorted=new SortedList(this);

	this->next=kernelSemHead;
	this->prev=0;
	if(kernelSemHead)
		kernelSemHead->prev=this;
	kernelSemHead=this;
}

KernelSem::~KernelSem(){
	if(this->next)
		this->next->prev=this->prev;
	if(this->prev)
		this->prev->next=this->next;
	else
		kernelSemHead=this->next;


	while(val<0)
		signal();

	delete blocked;
	delete sorted;
}

int KernelSem::wait(Time maxTimeToWait){
	if(--val<0){
		PCB::runningThread->threadState=BLOCKED;
		blocked->addMember(PCB::runningThread);
		sorted->addMember(PCB::runningThread,maxTimeToWait);
		dispatch();
		return PCB::runningThread->valueFromWait;
	}else
		return 1;
}

void KernelSem::signal(){
	if(val++<0){
		PCB* pcb=blocked->getMember();
		if(pcb){
			sorted->deleteMemberByPCB(pcb);
			pcb->threadState=READY;
			Scheduler::put(pcb);
			pcb->valueFromWait=1;
		}
	}
}
