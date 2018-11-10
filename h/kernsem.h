/*
 * kernsem.h
 *
 *  Created on: Apr 16, 2017
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_
#include "semaphor.h"
#include "list.h"

class KernelSem{
public:
	KernelSem(int initVal=1);
	~KernelSem();
	int wait(Time maxTimeToWait);
	void signal();

	int val;
	List* blocked;
	SortedList* sorted;
	static KernelSem* kernelSemHead;
	KernelSem* next;
	KernelSem* prev;
};

#endif /* KERNSEM_H_ */
