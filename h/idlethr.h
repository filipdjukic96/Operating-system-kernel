/*
 * idlethr.h
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#ifndef BCC_IDLETHR_H_
#define BCC_IDLETHR_H_

#include "thread.h"

class IdleThread:public Thread{
public:
	void run(){
		while(1) dispatch();
	}
};


#endif /* BCC_IDLETHR_H_ */
