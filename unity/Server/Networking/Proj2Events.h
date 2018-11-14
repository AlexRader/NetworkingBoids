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

#ifndef LAB3EVENTS_H
#define	LAB3EVENTS_H

#include <iostream>
#include <random>
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/GetTime.h"

//Events, event types, eventmanager - Rosser

class Queue;

//types of events
enum EventType {

	JOIN_EVENT,
	UPDATE_BLOID_STATE_EVENT,

};

//base class
class Event
{
public:
	Event();
	~Event();

	virtual void execute();

private:
	int mEventType;
};


//joins server event 
class JoinEvent : public Event
{
public:
	JoinEvent();
	JoinEvent(RakNet::RakPeerInterface *peer, char *ip, unsigned short port);
	~JoinEvent();

	void execute();

private:

	RakNet::RakPeerInterface *mPeer;

	EventType mEventType;

	char mUser[512];
	char mIP[512];

	unsigned short mPort;
};


class UpdateBloidEvent : public Event
{
public:
	UpdateBloidEvent();	
	UpdateBloidEvent(RakNet::RakPeerInterface *targetPeer, int id, float x, float y, float z, int dir, RakNet::Packet *pak);
	~UpdateBloidEvent();

	void execute();

private:

	RakNet::RakPeerInterface *peerInstance;
	RakNet::Packet *packet;
	float timeStamp;
	EventType mEventType;

	int objectId;
	float xPos, yPos, zPos;
	int direction;
	
};



//event manager with queue
class EventManager
{
public:
	EventManager();
	~EventManager();

	static EventManager *getInstance()
	{
		if (!mpInstance)
		{
			mpInstance = new EventManager;
		}
		return mpInstance;
	}

	void enqueueEvent(Event *eventToQ);
	void executeQueuedEvents();

private:
	Queue* mEventQ;

	static EventManager *mpInstance; // Alex Lab3 the singleton instance of the event system.
};



#endif