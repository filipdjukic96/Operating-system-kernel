/*
 * pcb.h
 *
 *  Created on: Apr 10, 2017
 *      Author: OS1
 */

#ifndef OS1PROJ_PCB_H_
#define OS1PROJ_PCB_H_
#define CREATED 0
#define READY 1
#define BLOCKED 2
#define FINISHED 3
#define PAUSED 4
#define lock asm{pushf;cli;}
#define unlock asm popf;
#include "thread.h"

const int maxNumOfHandlers=16;
typedef void interrupt (*timerInterrupt)(...);
void interrupt dispatch_();

class MainThread;
class Thread;
class IdleThread;
class SignalHandlersList;

class PCB{
public:
	static PCB* pcbHead;
	static PCB* getPCBbyID(ID id);

	static MainThread* mainThread;
	static PCB* runningThread;
	static PCB* idleThreadPCB;
	static IdleThread* idleThread;
	static int idleCreated;
	static void createIdle();
	static void deleteIdle();

	void start();
	PCB(StackSize stackSize,Time timeSlice,Thread* myThread);
	~PCB();
	static void runWrapper(Thread* myThread);
	void waitToComplete();
	void exitThread();
	ID getId();
	void initializeThread();


	friend void interrupt dispatch_();
	friend void interrupt timer(...);
	friend class Thread;
	friend class List;

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	SignalHandler getHandler(SignalId signal);

	void maskSignal(SignalId signal);
	static void maskSignalGlobally(SignalId signal);
	void unmaskSignal(SignalId signal);
	static void unmaskSignalGlobally(SignalId signal);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	static void pause();

	SignalHandler signalHandlers[maxNumOfHandlers];
	static int lockContextSwitch;
	static int dispatchCalled;
	static int globallyMaskedSignals[maxNumOfHandlers];
	static int globallyBlockedSignals[maxNumOfHandlers];
	int maskedSignals[maxNumOfHandlers];
	int blockedSignals[maxNumOfHandlers];


	static ID IDcount;
	ID myID;
	Thread* myThread;
	List* blockedThreads;
	SignalHandlersList* signalList;
	StackSize stackSize;
	Time timeSlice;
	unsigned int *myStack,ss,sp,bp;
	int threadState;
	static timerInterrupt oldTimer;
	static Time runtime;
	int valueFromWait;
	PCB* myParent;

	PCB* next;
	PCB* prev;

};



#endif /* OS1PROJ_PCB_H_ */
