/*
 * kernev.h
 *
 *  Created on: May 6, 2017
 *      Author: OS1
 */

#ifndef KERNEV_H_
#define KERNEV_H_

class PCB;
typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void signal();
	void wait();

	int val;
	IVTNo ivtNumber;
	PCB* myPCB;

};



#endif /* KERNEV_H_ */
