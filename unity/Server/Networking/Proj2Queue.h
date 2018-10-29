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
#ifndef LAB3QUEUE_H
#define LAB3QUEUE_H

#include "Proj2Events.h"
class Event;

//non STL queue, check it out!
class Queue
{
private:
	//Alex Lab3 gives me a typedef to use instead of node*, also contains data
	typedef struct Node
	{
		Event* mData;
		Node* nextNode;
	}* mNodePtr;

	mNodePtr mHead;
	mNodePtr mLast;
	mNodePtr mCurr;
	mNodePtr mTemp;

public:
	Queue();
	~Queue();


	void enQueue(Event* data);
	Event* deQueue();
	bool isEmpty();
};
#endif // !LAB3QUEUE_H

