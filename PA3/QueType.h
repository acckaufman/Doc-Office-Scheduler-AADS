//Amanda Kaufman
//CISS 350A
//Programming Assignment 3
//This is the QueType data structure implemented as a Linked List in Chapter 5 of our text.

#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstddef>	//For NULL
#include <new>		//For bad_alloc
#include "Scheduler.h"
using namespace std;

//Exception class for when a client attempts to push an element onto a full queue
class FullQueue
{
};

//Exception class for when a client attempts to pop an element off an empty queue
class EmptyQueue
{
};

//Structure to hold a patient's information.
struct Patient
{
	string name;
	int age;
	string specialty;
	bool emergency;
};

//Node structure for queue
struct NodeType
{
	Patient info;
	NodeType *next;
};

//Queue implemented as a linked list.
class QueType
{
private:
	NodeType *front;
	NodeType *rear;
public:
	//Function: Class constructor.
	//Post: Front and rear are set to null.
	QueType()
	{
		front = NULL;
		rear = NULL;
	}

	//Function: Empty the queue.
	//Post: Queue is empty; all elements have been deallocated.
	void makeEmpty()
	{
		NodeType *tempPtr;

		while (front != NULL)
		{
			tempPtr = front;
			front = front->next;
			delete tempPtr;
		}
		rear = NULL;
	}

	//Function: Class destructor.
	//Post: Queue is empty; all elements have been deallocated.
	~QueType()
	{
		makeEmpty();
	}

	//Function: Observe a patient in a waitlist, without removing the patient.
	//Pre: Waitlist queue has been initialized.  Index passed to function is not out of range.
	//Post: A copy of the patient at position index in the waitlist is returned.
	Patient getPatient(int index)
	{
		Patient temp;

		NodeType *tempPtr;

		tempPtr = front;
		for (int count = 0; count < index; count++)
		{
			tempPtr = tempPtr->next;
		}

		temp = tempPtr->info;
		return temp;
	}

	//Function: Adds a new patient to the rear of the queue (waitlist).
	//Pre: Waitlist queue has been initialized.
	//Post: If (queue is not full), newPatient is at the rear of the queue; otherwise,
	//a FullQueue exception is thrown.
	void enqueue(Patient newPatient)
	{
		if (isFull())
			throw FullQueue();
		else
		{
			NodeType *newNode;

			newNode = new NodeType;
			newNode->info = newPatient;
			newNode->next = NULL;
			if (rear == NULL)
				front = newNode;
			else
				rear->next = newNode;
			rear = newNode;
		}
	}

	//Function: Removes front patient from the waitlist and returns it in patient reference variable.
	//Pre: Waitlist queue has been initialized.
	//Post: If (queue is not empty), the front of the queue has been removed and a copy returned
	//in patient; otherwise, an EmptyQueue exception is thrown.
	void dequeue(Patient &patient)
	{
		if (isEmpty())
			throw EmptyQueue();
		else
		{
			NodeType *tempPtr;

			tempPtr = front;
			patient = front->info;
			front = front->next;
			if (front == NULL)
				rear = NULL;
			delete tempPtr;
		}
	}

	//Function: Determine whether or not the queue is empty.
	//Post: Returns true if there are no elements on the queue and false otherwise.
	bool isEmpty() const
	{
		return (front == NULL);
	}

	//Function: Determine whether or not the queue is full.
	//Post: Returns true if there is no room for another NodeType object on the free store,
	//and false otherwise.
	bool isFull() const
	{
		NodeType *location;
		try
		{
			location = new NodeType;
			delete location;
			return false;
		}
		catch (std::bad_alloc exception)
		{
			return true;
		}
	}
};

