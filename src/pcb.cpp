#include <iostream.h>
#include <dos.h>
#include "pcb.h"
#include "SCHEDULE.H"
#include "list.h"
#include "thread.h"
#include "kernsem.h"
#include "idlethr.h"
#include "handler.h"


int PCB::globallyMaskedSignals[maxNumOfHandlers]={0};
int PCB::globallyBlockedSignals[maxNumOfHandlers]={0};
int PCB::lockContextSwitch=0;
int PCB::dispatchCalled=0;
PCB* PCB::pcbHead=0;
MainThread* PCB::mainThread=0;
PCB* PCB::runningThread=0;
ID PCB::IDcount=0;
timerInterrupt PCB::oldTimer=0;
Time PCB::runtime=0;
PCB* PCB::idleThreadPCB=0;
IdleThread* PCB::idleThread=0;
int PCB::idleCreated=0;

void tick();



void PCB::signal(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;


	if (maskedSignals[signal]!=0)	return;
	if (globallyMaskedSignals[signal]!=0)	return;

	if(threadState==PAUSED){
		threadState=READY;
		Scheduler::put(this);
	}

	if(signalHandlers[signal]==0) return;

    signalList->addSignal(signal);


}

void PCB::registerHandler(SignalId signal, SignalHandler handler){
	if(signal<=0 ) return;
	if(signal>=maxNumOfHandlers) return;
	signalHandlers[signal]=handler;
 }

SignalHandler PCB:: getHandler(SignalId signal){
	if(signal<0) return 0;
	if(signal>=maxNumOfHandlers) return 0;
	return signalHandlers[signal];
 }

void PCB::pause(){
	PCB::runningThread->threadState=PAUSED;
	dispatch();
}

void PCB::blockSignal(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	blockedSignals[signal]=1;
}

void PCB::blockSignalGlobally(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	globallyBlockedSignals[signal]=1;
}

void PCB::unblockSignal(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	blockedSignals[signal]=0;
}

void PCB::unblockSignalGlobally(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	globallyBlockedSignals[signal]=0;
}

void PCB::maskSignal(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	maskedSignals[signal]=1;
}

void PCB::maskSignalGlobally(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	globallyMaskedSignals[signal]=1;
}

void PCB::unmaskSignal(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	maskedSignals[signal]=0;
}

void PCB::unmaskSignalGlobally(SignalId signal){
	if(signal<0) return;
	if(signal>=maxNumOfHandlers) return;
	globallyMaskedSignals[signal]=0;
}


PCB* PCB::getPCBbyID(ID id){
	PCB* pcb=pcbHead;
	while(pcb){
		if(pcb->myID==id) return pcb;
		pcb=pcb->next;
	}
	return 0;
}

PCB::PCB(StackSize stackSize,Time timeSlice,Thread* myThread){

	this->prev=0;
	this->next=pcbHead;
	if(pcbHead)
		pcbHead->prev=this;
	pcbHead=this;

	myParent=PCB::runningThread;

	myStack=0;
	threadState=CREATED;

	if(stackSize>maxStackSize)
		stackSize=maxStackSize;

	this->stackSize=stackSize/sizeof(unsigned int);
	this->timeSlice=timeSlice;
	this->myThread=myThread;
	blockedThreads=new List();
	signalList=new SignalHandlersList();
	myID=IDcount++;
	if(idleCreated)
		idleThreadPCB=this;


	for(int i=0;i<maxNumOfHandlers;i++)
		if(runningThread!=0){
			blockedSignals[i]=runningThread->blockedSignals[i];
			maskedSignals[i]=runningThread->maskedSignals[i];
			signalHandlers[i]=runningThread->signalHandlers[i];
		} else{
			blockedSignals[i]=maskedSignals[i]=0;
			signalHandlers[i]=0;
		}

	if(runningThread==0)
		signalHandlers[0]=signalHandlerZero;


}

PCB::~PCB(){
  if(this->next)
		this->next->prev=this->prev;
	if(this->prev)
		this->prev->next=this->next;
	else
		pcbHead=this->next;

	threadState=FINISHED;
	delete blockedThreads;
	delete signalList;
	delete[] myStack;
}

void PCB::start(){
	if(threadState!=CREATED) return;
	threadState=READY;
	initializeThread();
	if(this!= PCB::idleThreadPCB)
		Scheduler::put(this);
}

ID PCB::getId(){
	return myID;
}

void PCB::exitThread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	List::Member* p=blockedThreads->first;

	while(p!=0){
		PCB* thread=p->pcb;
		thread->threadState=READY;
		Scheduler::put(thread);
		p=p->next;
	}
	this->threadState=FINISHED;
	delete blockedThreads;
	blockedThreads=0;

	if(myParent)
		myParent->signal(1);


	if(signalHandlers[2]!=0 && maskedSignals[2]!=1 && globallyMaskedSignals[2]!=1 && blockedSignals[2]!=0 && globallyBlockedSignals[2]!=1)
		(*signalHandlers[2])();

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	dispatch();
}

void PCB::waitToComplete(){
	if(this->threadState==CREATED) return;
	if(this->threadState==FINISHED) return;
	if(this==PCB::runningThread) return;
	if(this==PCB::idleThreadPCB) return ;
	runningThread->threadState=BLOCKED;
	blockedThreads->addMember(runningThread);
	dispatch();
}

void PCB::runWrapper(Thread* myThread){
	myThread->run();
	myThread->exitThread();
}

void PCB::initializeThread(){

	myStack=new unsigned[stackSize];

#ifndef BCC_BLOCK_IGNORE
	myStack[stackSize-1]=FP_SEG(myThread);
	myStack[stackSize-2]=FP_OFF(myThread);
#endif
	myStack[stackSize-3]=0;
	myStack[stackSize-4]=0;

	myStack[stackSize-5]=0x200;
#ifndef BCC_BLOCK_IGNORE
	myStack[stackSize-6]=FP_SEG(&runWrapper);
	myStack[stackSize-7]=FP_OFF(&runWrapper);
#endif


#ifndef BCC_BLOCK_IGNORE
	sp=FP_OFF(&myStack[stackSize-16]);
	ss=FP_SEG(&myStack[stackSize-16]);
#endif
	bp=sp;
}

unsigned int ssHelp,spHelp,bpHelp;

void interrupt dispatch_(){


	if(PCB::lockContextSwitch==1){
		PCB::dispatchCalled=1;
		return;
	}else
		PCB::dispatchCalled=0;


#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ssHelp,ss
		mov spHelp,sp
		mov bpHelp,bp
	}
#endif

	PCB::runningThread->bp=bpHelp;
	PCB::runningThread->ss=ssHelp;
	PCB::runningThread->sp=spHelp;



	if(PCB::runningThread->threadState!=BLOCKED && PCB::runningThread->threadState!=FINISHED && PCB::runningThread!=PCB::idleThreadPCB && PCB::runningThread->threadState!=PAUSED)
		Scheduler::put(PCB::runningThread);

	PCB::runningThread=Scheduler::get();

	if(PCB::runningThread==0)
			PCB::runningThread=PCB::idleThreadPCB;



	ssHelp=PCB::runningThread->ss;
	spHelp=PCB::runningThread->sp;
	bpHelp=PCB::runningThread->bp;

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss,ssHelp
		mov sp,spHelp
		mov bp,bpHelp
	}
#endif

	PCB::runtime=0;

	SignalHandlersList::handleSignals();




}


void interrupt timer(...){

	if(PCB::runningThread->timeSlice>0)
		PCB::runtime++;
	tick();
	(*PCB::oldTimer)();
	KernelSem* kernelSem=KernelSem::kernelSemHead;
	while(kernelSem){
		kernelSem->sorted->updateSortedList();
		kernelSem=kernelSem->next;
	}
if(PCB::runningThread->timeSlice > 0 && PCB::runtime >= PCB::runningThread->timeSlice)
	dispatch();
}

void PCB::createIdle(){
	idleCreated=1;
	idleThread=new IdleThread();
	idleCreated=0;
	idleThread->start();
}

void PCB::deleteIdle(){
	delete idleThread;
	idleThreadPCB=0;
	idleThread=0;
}
