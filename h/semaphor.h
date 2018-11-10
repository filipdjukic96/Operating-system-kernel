/*
 * semaphor.h
 *
 *  Created on: Apr 16, 2017
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int initVal=1);
	virtual ~Semaphore();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val() const;
	friend class KernelSem;
private:
	KernelSem* myImpl;
};




#endif /* SEMAPHOR_H_ */
