/*
 * thread.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.H"


void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	dispatch_();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::signal(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->signal(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->registerHandler(signal,handler);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
 }

SignalHandler Thread:: getHandler(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	SignalHandler ret=myPCB->getHandler(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ret;
 }

void Thread::pause(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::pause();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::blockSignal(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->blockSignal(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::blockSignalGlobally(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::blockSignalGlobally(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::unblockSignal(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->unblockSignal(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::unblockSignalGlobally(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::unblockSignalGlobally(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::maskSignal(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->maskSignal(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::maskSignalGlobally(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::maskSignalGlobally(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::unmaskSignal(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->unmaskSignal(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::unmaskSignalGlobally(SignalId signal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::unmaskSignalGlobally(signal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread::Thread(StackSize stackSize,Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB=new PCB(stackSize,timeSlice,this);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread::~Thread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

ID Thread::getId(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ID threadId= myPCB->getId();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return threadId;
}

Thread* Thread::getThreadById(ID id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB* pcb=PCB::getPCBbyID(id);
	Thread* thr;
	if(pcb!=0)
		thr=pcb->myThread;
	else thr=0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return thr;
}

void Thread::start(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->start();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

ID Thread::getRunningId(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ID runningId=PCB::runningThread->getId();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return runningId;
}

void Thread::exitThread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->exitThread();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

}

void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->waitToComplete();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


