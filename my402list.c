/*
 * Author:      William Chia-Wei Cheng (bill.cheng@usc.edu)
 *
 * @(#)$Id: my402list.h,v 1.2 2020/05/18 05:09:12 william Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"
#include "my402list.h"


int  My402ListLength(My402List* myList){
	return myList->num_members;
}
int  My402ListEmpty(My402List* myList){
	if(myList->num_members==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int  My402ListAppend(My402List* myList, void* obj){

	My402ListElem *elem_ptr = (My402ListElem*)malloc(sizeof(My402ListElem));
	elem_ptr->obj = obj;
	if(myList->num_members==0)
	{
		elem_ptr->next = &(myList->anchor);
		elem_ptr->prev = &(myList->anchor);

		(myList->anchor).next = elem_ptr;
		(myList->anchor).prev = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	else
	{
		My402ListElem* myLastElement = (myList->anchor).prev;
		elem_ptr->next = &(myList->anchor);
		elem_ptr->prev = myLastElement;
		myLastElement->next = elem_ptr;
		(myList->anchor).prev = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	return FALSE;
}

int  My402ListPrepend(My402List* myList, void* obj){

	My402ListElem *elem_ptr = (My402ListElem*)malloc(sizeof(My402ListElem));
	elem_ptr->obj = obj;

	if(myList->num_members==0)
	{
		elem_ptr->next = &(myList->anchor);
		elem_ptr->prev = &(myList->anchor);

		(myList->anchor).next = elem_ptr;
		(myList->anchor).prev = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	else
	{
		My402ListElem* myFirstElement = My402ListFirst(myList);
		(myList->anchor).next = elem_ptr;
		elem_ptr->next = myFirstElement;
		elem_ptr->prev = &(myList->anchor);
		myFirstElement->prev = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	return FALSE;
}

void My402ListUnlink(My402List *myList, My402ListElem *myListElem){
	My402ListElem *prevElem, *nextElem;
	prevElem = myListElem->prev;
	nextElem = myListElem->next;

	prevElem->next = nextElem;
	nextElem->prev = prevElem;
	myList->num_members = myList->num_members - 1;

	free(myListElem);
}

void My402ListUnlinkAll(My402List *myList){
	
	My402ListElem *elem_ptr=NULL;
	for (elem_ptr=My402ListFirst(myList); elem_ptr != NULL; elem_ptr=My402ListNext(myList, elem_ptr)) 
    {
    	// if(elem_ptr == NULL)
    	// {
    	// 	return NULL;
    	// }

    	My402ListUnlink(myList, elem_ptr);
    }
    My402ListInit(myList);

}

int  My402ListInsertAfter(My402List* myList, void* obj, My402ListElem* myListElem){

	if(myList->num_members == 0)
	{
		My402ListAppend(myList, obj);
		return TRUE;
	}
	else
	{
		My402ListElem *elem_ptr = (My402ListElem*)malloc(sizeof(My402ListElem));
		elem_ptr->obj = obj;
		My402ListElem* nextElement = myListElem->next;
		elem_ptr->next = myListElem->next;
		elem_ptr->prev = myListElem;
		myListElem->next = elem_ptr;
		nextElement->prev = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	return FALSE;
}

int  My402ListInsertBefore(My402List* myList, void* obj, My402ListElem* myListElem){

	if(myList->num_members == 0)
	{
		My402ListPrepend(myList, obj);
		return TRUE;
	}
	else
	{
		My402ListElem *elem_ptr = (My402ListElem*)malloc(sizeof(My402ListElem));
		elem_ptr->obj = obj;
		My402ListElem* previousElement = myListElem->prev;
		elem_ptr->next = myListElem;
		elem_ptr->prev = previousElement;
		myListElem->prev = elem_ptr;
		previousElement->next = elem_ptr;
		(myList->num_members)++;
		return TRUE;
	}
	return FALSE;
}

My402ListElem *My402ListFirst(My402List* myList){
	if(myList->num_members == 0)
	{
		return NULL;
	}
	else
	{
		return (myList->anchor).next;
	}
}

My402ListElem *My402ListLast(My402List* myList){
	
	if(myList->num_members == 0)
	{
		return NULL;
	}
	else
	{
		return (myList->anchor).prev;
	}
}

My402ListElem* My402ListNext(My402List* myList, My402ListElem* myListElem){

	if(myListElem->next == &(myList->anchor))
	{
		return NULL;
	}
	return myListElem->next;
}

My402ListElem *My402ListPrev(My402List* myList, My402ListElem* myListElem){

	if(myListElem->prev == &(myList->anchor))
	{
		return NULL;
	}
	return myListElem->prev;
}

My402ListElem *My402ListFind(My402List* myList, void* obj){

	My402ListElem *elem_ptr=NULL;

    for (elem_ptr=My402ListFirst(myList); elem_ptr != NULL; elem_ptr=My402ListNext(myList, elem_ptr)) 
    {
    	// if(elem_ptr == NULL)
    	// {
    	// 	return NULL;
    	// }

    	if(elem_ptr->obj == obj)
    	{
    		return elem_ptr;
    	}
    }
	return NULL;
}

int My402ListInit(My402List* myList){
	
	if(myList == NULL)
	{
		return FALSE;
	}
	else
	{
		myList->num_members = 0;
		(myList->anchor).obj = NULL;
		(myList->anchor).prev = &(myList->anchor);
		(myList->anchor).next = &(myList->anchor);
		return TRUE;		
	}
}
