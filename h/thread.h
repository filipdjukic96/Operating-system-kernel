#ifndef OS1PROJ_THREAD_H_
#define OS1PROJ_THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize=4096;
const StackSize maxStackSize=65536;
typedef unsigned int Time;
const Time defaultTimeSlice=2;
typedef int ID;
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class PCB;

class Thread{
public:
	void start();
	void waitToComplete();
	void exitThread();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

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

protected:
	friend class PCB;
	Thread(StackSize stackSize=defaultStackSize,Time timeSlice=defaultTimeSlice);
	virtual void run(){}
private:
	PCB* myPCB;
};

void dispatch();


#endif /* OS1PROJ_THREAD_H_ */
