#include "list.h"
#include "pcb.h"
#include "kernsem.h"
#include "schedule.h"


List::List():first(0),last(0),numOfMembers(0){}
List::~List(){

	Member* p=first;
	while(p!=0){
		first=first->next;
		delete p;
		p=first;
	}
	last=0;
	numOfMembers=0;
}

void List::addMember(PCB* pcb){
	Member* newMember=new Member(pcb);
	if(first==0) first=newMember;
	else last->next=newMember;
	last=newMember;
	numOfMembers++;
}

PCB* List::getMember(){
	Member* oldMember=first;
	if(oldMember==0) return 0;
	first=first->next;
	if(first==0) last=0;
	PCB* pcb=oldMember->pcb;
	numOfMembers--;
	delete oldMember;
	return pcb;
}

int List::getNumOfMembers()const{
	return numOfMembers;
}

PCB* List::getPCBbyID(ID id){
	Member* p=first;
	while(p!=0){
		if(p->pcb->myID==id) return p->pcb;
		p=p->next;
	}
	return 0;
}

void List::deleteMember(ID id){
	Member* p=first;
	Member* prev=0;

	while(p!=0 && p->pcb->myID!=id){
		prev=p;
		p=p->next;
	}

	if(p!=0){
		if(prev==0){
		first=p->next;
		if(first==0) last=0;
		}
	else prev->next=p->next;
	if(p==last)
		last=prev;
	numOfMembers--;
	delete p;
	}
}


SortedList::SortedList(KernelSem* ksem):first(0),numOfMembers(0),kernelSem(ksem){}
SortedList::~SortedList(){
	Member* p=first;
		while(p!=0){
			first=first->next;
			delete p;
			p=first;
		}
		numOfMembers=0;
}

int SortedList::getNumOfMembers()const{
	return numOfMembers;
}


void SortedList::addMember(PCB* pcb,Time time){

	if(pcb==0) return;
	if(time==0) return;



	Member* newElem=new Member(pcb,time);
	Member* p=first;
	Member* prev=0;

	while((p!=0) && (p->time<=newElem->time)){
		newElem->time-=p->time;
		prev=p;
		p=p->next;
	}

	numOfMembers++;

	if(p!=0){
		newElem->next=p;
		p->time-=newElem->time;
	}

	if(prev==0)
		first=newElem;
	else
		prev->next=newElem;


}


void SortedList::deleteMemberByPCB(PCB* pcb){
	Member* p=first;
	if(p==0) return;
	Member* prev=0;
	while ((p!=0) && (p->pcb!=pcb)){
		prev = p;
		p = p->next;
	}
	if (p!=0){
		if (p->next && p->time)
			p->next->time += p->time;
		if(!prev)
			first=p->next;
		else
			prev->next = p->next;
		delete p;
		numOfMembers--;
	}

}

void SortedList::updateSortedList(){
	Member* p=first;
	if (p==0)
		return;
	p->time--;
	while(p && p->time==0){
		first = p->next;
		p->pcb->threadState = READY;
		Scheduler::put(p->pcb);
		kernelSem->blocked->deleteMember(p->pcb->myID);
		kernelSem->val++;
		p->pcb->valueFromWait=0;
		delete p;
		numOfMembers--;
		p = first;
	}

}

