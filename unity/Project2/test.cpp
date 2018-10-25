
#include "test.h"

//#define BloidInfo// __declspec (dllexport)

//#pragma pack(push, 1)
//struct BloidMessage {
//
//	char typeID;
//
//	int objectId;
//	float x, y, z;
//	int direction;
//
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct BloidData {
//
//	int objectId;
//	float x, y, z;
//	int direction;
//
//};
//#pragma pack(pop)

//client plugin
RakNet::RakPeerInterface *gpPeerInstance = RakNet::RakPeerInterface::GetInstance();



	//connect
	void connectToServer(char* ip)
	{
		RakNet::SocketDescriptor sd;
		gpPeerInstance->Startup(1, &sd, 1);

		char str[512];
	
		unsigned short serverPort = 1111;
	
		strcpy(str, "216.93.149.128");
	
		//RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	
		gpPeerInstance->Connect(str, serverPort, 0, 0);

	}

	//receive data
	BloidData receiveData()
	{
		RakNet::Packet *packet;

		
			packet = gpPeerInstance->Receive();
		
			switch (packet->data[0])
			{
			case 135:
			

				const BloidMessage *newBloidData = (BloidMessage *)packet->data;

				//return (char*)packet->data;

				BloidData newData;

				newData.objectId = newBloidData->objectId;
				newData.x = newBloidData->x;
				newData.y = newBloidData->y;
				newData.z = newBloidData->z;
				newData.direction = newBloidData->direction;

				return newData;
			
				break;
			}

		
	}


		//send data back to server
	void sendData(int id, float x, float y, float z, int dir, char* serverGuid)
	{

		//message as "game message type"
		BloidMessage bloid;
		
		bloid.objectId = 135;
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
