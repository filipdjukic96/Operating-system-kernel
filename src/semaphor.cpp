/*
 * semaphor.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: OS1
 */
#include "pcb.h"
#include "kernsem.h"
#include "semaphor.h"

Semaphore::Semaphore(int initVal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl=new KernelSem(initVal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Semaphore::~Semaphore(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::wait(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int retValue = myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return retValue;
}

void Semaphore::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::val()const{
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int value = myImpl->val;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return value;
}




