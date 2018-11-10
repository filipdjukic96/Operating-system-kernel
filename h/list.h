/*
 * list.h
 *
 *  Created on: Apr 10, 2017
 *      Author: OS1
 */

#ifndef OS1PROJ_LIST_H_
#define OS1PROJ_LIST_H_
#include "thread.h"
class PCB;
class KernelSem;

class SortedList{
public:
	void addMember(PCB* pcb,Time time);
	void deleteMemberByPCB(PCB* pcb);
	void updateSortedList();
	int getNumOfMembers()const;


	SortedList(KernelSem* kernelSem);
	~SortedList();

	struct Member{
				PCB* pcb;
				Member* next;
				Time time;
				Member(PCB* pcb_,Time time_){pcb=pcb_;time=time_;next=0;}
			};

	int numOfMembers;
	Member* first;
	KernelSem* kernelSem;
};


class List{
public:
	void addMember(PCB* pcb);
	PCB* getMember();
	void deleteMember(ID id);
	int getNumOfMembers()const;
	List();
	~List();
	PCB* getPCBbyID(ID id);

	struct Member{
			PCB* pcb;
			Member* next;
			Member(PCB* pcb_){pcb=pcb_;next=0;}
		};

	int numOfMembers;
	Member *first;
	Member *last;


};






#endif /* OS1PROJ_LIST_H_ */
