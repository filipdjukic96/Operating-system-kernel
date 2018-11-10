#include "pcb.h"
#include <iostream.h>
#include <stdlib.h>
#ifndef BCC_BLOCK_IGNORE
#include <dos.h>
#endif
int userMain(int argc, char* argv[]);

void _FARFUNC operator delete(void* ptr){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	free(ptr);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}



void _FAR* _FARFUNC operator new( unsigned size){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	void* ptr=malloc(size);

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ptr;
}




class MainThread:public Thread{
public:
	MainThread(int argc_,char* argv_[]):argc(argc_),argv(argv_),ret(0){}
	virtual ~MainThread(){waitToComplete();}
	void run(){ret=userMain(argc,argv);}
	int retVal()const{return ret;}

private:
	int argc,ret;
	char** argv;
};



int main(int argc, char* argv[]){


	PCB::runningThread = new PCB(defaultStackSize, defaultTimeSlice, 0);
	PCB::runningThread->threadState = READY;

	PCB::createIdle();

	PCB::mainThread=new MainThread(argc,argv);

	PCB::mainThread->start();

#ifndef BCC_BLOCK_IGNORE
	PCB::oldTimer = getvect(8);
	setvect(8, &timer);
#endif

	PCB::mainThread->waitToComplete();

#ifndef BCC_BLOCK_IGNORE
	setvect(8, PCB::oldTimer);
#endif


	int ret = PCB::mainThread->retVal();

	delete	PCB::mainThread;
	PCB::deleteIdle();
	delete PCB::runningThread;

	return ret;
}

