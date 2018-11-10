/*
 * handler.cpp
 *
 *  Created on: May 16, 2017
 *      Author: OS1
 */

#include "pcb.h"
#include "handler.h"



void signalHandlerZero(){
	PCB::runningThread->exitThread();
}


void SignalHandlersList::addSignal(SignalId signal){
	Member* newMember=new Member(signal);
	newMember->prev=last;
	if(first==0)
		first=newMember;
	else last->next=newMember;
	last=newMember;
	numOfMembers++;
}

void SignalHandlersList::removeSignal(Member* p){

	if(p->prev)
		p->prev->next=p->next;
	else first=p->next;
	if(p->next)
		p->next->prev=p->prev;
	else last=p->prev;

	delete p;
}

void SignalHandlersList::handleSignals(){

	PCB::lockContextSwitch=1;
#ifndef BCC_BLOCK_IGNORE
	asm { sti; }
#endif

	Member* p=PCB::runningThread->signalList->first;
	while(p!=0){
		Member* rem=p;
		p=p->next;
		SignalId signal=rem->signal;
		if(PCB::runningThread->blockedSignals[signal]==0 && PCB::globallyBlockedSignals[signal]==0){
			PCB::runningThread->signalList->removeSignal(rem);
			if(PCB::runningThread->signalHandlers[signal]!=0)
				(*PCB::runningThread->signalHandlers[signal])();
		}
	}

#ifndef BCC_BLOCK_IGNORE
	asm { cli; }
#endif
	PCB::lockContextSwitch=0;
	if(PCB::dispatchCalled==1)
		dispatch();
}

SignalHandlersList::SignalHandlersList(){
	first=last=0;
	numOfMembers=0;
}

SignalHandlersList::~SignalHandlersList(){
	while(first!=0)
		removeSignal(first);
}

