/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Project2 | due 10/29/2018
current date 10/3/2018

“We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.”
*/
#include "test.h"

//client plugin
RakNet::RakPeerInterface *gpPeerInstance;// = RakNet::RakPeerInterface::GetInstance();

void raknetPeer()
{
	gpPeerInstance = RakNet::RakPeerInterface::GetInstance();
}
//connect
void connectToServer(char* ip)
{
	RakNet::SocketDescriptor sd;
	gpPeerInstance->Startup(1, &sd, 1);

	char str[512];

	unsigned short serverPort = 1111;

	strcpy(str, ip);

	gpPeerInstance->Connect(str, serverPort, 0, 0);

}

//receive data (Project2)
BloidData receiveData()
{
	RakNet::Packet *packet;

	for (packet = gpPeerInstance->Receive(); packet; gpPeerInstance->DeallocatePacket(packet), packet = gpPeerInstance->Receive())
	{
		switch (packet->data[0])
		{
		case ID_GAME_MESSAGE_1:
		{
			const BloidMessage *newBloidData = (BloidMessage *)packet->data;


			BloidData newData;

			newData.objectId = newBloidData->objectId;
			newData.timeStamp = newBloidData->timeStamp;
			newData.x = newBloidData->x;
			newData.y = newBloidData->y;
			newData.z = newBloidData->z;
			newData.direction = newBloidData->direction;

			return newData;
		}
			break;
		default:
		{
			BloidData newData;
			return newData;
		}
			break;
		}
	}
}


	//send data back to server (Project2)
void sendData(int id, float tStamp, float x, float y, float z, int dir)
{

	//message as "game message type"
	BloidMessage bloid;
	
	bloid.typeID = ID_GAME_MESSAGE_4;

	bloid.timeStamp = tStamp;
	bloid.objectId = id;
	bloid.x = x;
	bloid.y = y;
	bloid.z = z;
	bloid.direction = dir;

	gpPeerInstance->Send((char *)&bloid, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpPeerInstance->GetSystemAddressFromIndex(0), false);

}

void kickRequest()
{
	return;
}
// (Project2) unit test for checking if bloids data was being sent by server/ recieve correctly
// it wasnt, fixed error, i cried with tears of joy
BloidData Test()
{
	BloidData tester;
	tester.objectId = -2;
	tester.timeStamp = -1;
	tester.direction = 2;
	tester.x = 1;
	tester.y = 3;
	tester.z = 4;

	return tester;
}
// (Project2) data sent by the server on start up, will be used by unity to get propper initial state
BloidData InitialData()
{
	RakNet::Packet *packet;

	for (packet = gpPeerInstance->Receive(); packet; gpPeerInstance->DeallocatePacket(packet), packet = gpPeerInstance->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			BloidData dat = Test();
			return dat;
		}
		break;
		case ID_GAME_MESSAGE_2:
		{
			const BloidMessage *newBloidData = (BloidMessage *)packet->data;

			BloidData newData;

			newData.objectId = newBloidData->objectId;
			newData.timeStamp = newBloidData->timeStamp;
			newData.x = newBloidData->x;
			newData.y = newBloidData->y;
			newData.z = newBloidData->z;
			newData.direction = newBloidData->direction;

			return newData;
		}
		break;
		case ID_GAME_MESSAGE_3:
		{
			const BloidMessage *newBloidData = (BloidMessage *)packet->data;
			BloidData newData;

			newData.objectId = -1;

			return newData;
		}
		break;
		default:
		{
			BloidData newData;

			newData.objectId = -2;

			return newData;
		}
		break;
		}
	}
}
