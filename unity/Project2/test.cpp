
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

	strcpy(str, "216.93.149.120");

	gpPeerInstance->Connect(str, serverPort, 0, 0);

}

//receive data
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

			//return (char*)packet->data;

			BloidData newData;

			newData.objectId = newBloidData->objectId;
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


	//send data back to server
void sendData(int id, float x, float y, float z, int dir, char* serverGuid)
{

	//message as "game message type"
	BloidMessage bloid;
	
	bloid.objectId = 204;
	bloid.x = x;
	bloid.y = y;
	bloid.z = z;
	bloid.direction = dir;

	gpPeerInstance->Send((char *)&bloid, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::RakPeerInterface::GetInstance()->GetGUIDFromIndex(0), true);

}

void kickRequest()
{
	return;
}

BloidData Test()
{
	BloidData tester;
	tester.objectId = -2;
	tester.direction = 2;
	tester.x = 1;
	tester.y = 3;
	tester.z = 4;

	return tester;
}

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
