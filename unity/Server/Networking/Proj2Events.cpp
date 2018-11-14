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
#include "Proj2Events.h"
#include "Proj2Queue.h"
#include "Data.h"
///


//BASE EVENT
Event::Event()
{

}
Event::~Event()
{

}
void Event::execute()
{
	printf("\n base event firing \n");
}
///

//JOIN SERVER EVENT
JoinEvent::JoinEvent()
{
	mEventType = JOIN_EVENT;
}
JoinEvent::JoinEvent(RakNet::RakPeerInterface *peer, char* ip, unsigned short port)
{
	mPeer = peer;
	strcpy(mIP, ip);
	mPort = port;

	printf("\n JOIN EVENT CREATED! \n");
}

JoinEvent::~JoinEvent()
{

}
void JoinEvent::execute()
{
	//join server with member information
	mPeer->Connect(mIP, mPort, 0, 0);
}


UpdateBloidEvent::UpdateBloidEvent()
{
	mEventType = UPDATE_BLOID_STATE_EVENT;
}

UpdateBloidEvent::UpdateBloidEvent(RakNet::RakPeerInterface *peer, int id, float x, float y, float z, int dir, RakNet::Packet *pak)
{
	//dont set timestamp til sent
	//timeStamp = RakNet::GetTime();
	mEventType = UPDATE_BLOID_STATE_EVENT;

	peerInstance = peer;
	objectId = id;
	xPos = x;
	yPos = y;
	zPos = z;
	direction = dir;
	packet = pak;
}

void UpdateBloidEvent::execute()
{

	RakNet::Packet *newPacket;
	//send positions of boid to client

	BloidMessage myBloidMessage[1];


	myBloidMessage->typeID = ID_GAME_MESSAGE_4;

	myBloidMessage->objectId = objectId;
	myBloidMessage->x = xPos;
	myBloidMessage->y = yPos;
	myBloidMessage->z = zPos;
	myBloidMessage->direction = direction;

	myBloidMessage->timeStamp = RakNet::GetTimeMS();

	peerInstance->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

}

UpdateBloidEvent::~UpdateBloidEvent()
{

}




//Eventmanager with member queue
EventManager::EventManager()
{
	mEventQ = new Queue;
}

EventManager::~EventManager()
{
	while (!mEventQ->isEmpty())
	{
		mEventQ->deQueue();
	}
	delete mEventQ;
}

//enqueues pointer to event
void EventManager::enqueueEvent(Event *eventToQ)
{
	mEventQ->enQueue(eventToQ);
}

//execute all events
void EventManager::executeQueuedEvents()
{
	Event* tmp;

	if (!mEventQ->isEmpty())
	{
		tmp = mEventQ->deQueue();

		tmp->execute();
	}
}
