/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Lab 3 | due 10/3/2018
current date 10/3/2018

“We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.”

This file was added to the project for access to an event and queue system
*/
#include "Proj2Queue.h"

//Alex Lab3 init the vars
Queue::Queue()
{
	mHead =	mLast = mTemp = nullptr;
}

//Alex lab3 clean-up
Queue::~Queue()
{
	delete mHead;
	delete mLast;
	delete mTemp;
}

//Alex lab3 standard enqueue
void Queue::enQueue(Event* data)
{
	mNodePtr newNode = new Node;

	newNode->nextNode = nullptr;
	newNode->mData = data;

	if (mHead == nullptr)
	{
		mHead = newNode;
		mLast = newNode;
		mHead->nextNode = nullptr;
	}
	else
	{
		mLast->nextNode = newNode;
		mLast = newNode;
	}
	mLast->nextNode = nullptr;
}

//Alex Lab3 standard dequeue
Event* Queue::deQueue()
{
	Event* popEvent;

	
	
	popEvent = mHead->mData;
	if (mHead->nextNode == nullptr)
	{
		mHead = nullptr;
		mLast = nullptr;
	}
	else
	{
		mTemp = mHead;
		mHead = mHead->nextNode;
		mTemp->nextNode = nullptr;
		delete mTemp;
	}
	
	return popEvent;
}

//Alex Lab3 Check empty.
bool Queue::isEmpty()
{
	if (mHead != nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}