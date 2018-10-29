
//Base code taken from 
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample1.html
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample2.html
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html
//http://www.cplusplus.com/reference/cstdlib/strtoul/
//
/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Lab 3 | due 10/3/2018
current date 10/3/2018

“We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.”
*/

#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     
#include <time.h>       
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include "RakNet/GetTime.h"

#include "Proj2Events.h"
#include "Proj2Queue.h"
#include "Data.h"
#include "BloidSerializer.h"

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GAME_MESSAGE_2 = ID_USER_PACKET_ENUM + 2,
	ID_GAME_MESSAGE_3 = ID_USER_PACKET_ENUM + 3,
	ID_GAME_MESSAGE_4 = ID_USER_PACKET_ENUM + 4 // recieve client boids
};


#pragma pack(push, 1)
struct customMessage
{
	GameMessages typeId = ID_GAME_MESSAGE_3;
	char messageStr[512];
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BloidMessage 
{
	//GameMessages typeId = ID_GAME_MESSAGE_1;
	char typeID = ID_GAME_MESSAGE_1;

	//BloidData sentBloid;

	int objectId;
	float x, y, z;
	int direction;

};
#pragma pack(pop)

// needed for RakNet Defined classes, and structs 
using namespace RakNet;

int main(void)
{
	const float SERVER_UPDATE = 100;
	std::vector <Bloid> bloids;
	std::vector <RakNet::RakNetGUID> IDsToUpdate;

	float timePrev, timeServerUpdate,timeCurr;
	customMessage myModifyMessage[1];
	BloidMessage myBloidMessage[1];

	unsigned int maxClients; 
	unsigned int serverType;
	unsigned short serverPort = 1111;

	char str[512];
	
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer = true;
	RakNet::Packet *packet;

	str[0] = '0';// this will need to be initialized for later down the line


	//get the maximum number of clients 
	printf("Please enter the max clients you wish to have\n");
	fgets(str, 512, stdin);

	maxClients = (unsigned short) strtoul(str, NULL, 0);

	printf("Please enter the type of server (1 = push, 2 = share, 3 = couple)\n");
	fgets(str, 512, stdin);
	serverType = (unsigned short)strtoul(str, NULL, 0);
	if (serverType > 3 || serverType < 1)
	{
		serverType = 1;
	}

	//modified with local variables
	SocketDescriptor sd(serverPort, 0);
	peer->Startup(maxClients, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(maxClients); //modified with local variables

	timePrev = RakNet::GetTimeMS();
	timeServerUpdate = RakNet::GetTimeMS();

	if (serverType == 1)
	{
		for (int bl = 0; bl < 4; ++bl)
		{
			Bloid newBloid(bl, 0, 0, 0, -1, RakNet::GetTime());
			newBloid.setBloidDirection();

			bloids.push_back(newBloid);
		}
	}

	while (1)
	{
		timeCurr = RakNet::GetTimeMS();

		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");
				myBloidMessage->typeID = ID_GAME_MESSAGE_2;
				if (serverType == 1)
				{
					for (int i = 0; i < bloids.size(); ++i)
					{
						myBloidMessage->objectId = bloids.at(i).objectId;
						myBloidMessage->x = bloids.at(i).x;
						myBloidMessage->y = bloids.at(i).y;
						myBloidMessage->z = bloids.at(i).z;
						myBloidMessage->direction = bloids.at(i).direction;
						peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
				}
				else if (serverType >= 2)
				{
					for (int i = 0; i < bloids.size(); ++i)
					{
						myBloidMessage->objectId = bloids.at(i).objectId;
						myBloidMessage->x = bloids.at(i).x;
						myBloidMessage->y = bloids.at(i).y;
						myBloidMessage->z = bloids.at(i).z;
						myBloidMessage->direction = bloids.at(i).direction;
						peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					myModifyMessage->typeId = ID_GAME_MESSAGE_3;
					peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					for (int i = 0; i < 4; ++i)
					{
						Bloid newBloid(bloids.size(), 0, 0, 0, -1, RakNet::GetTime());
						newBloid.setBloidDirection();

						bloids.push_back(newBloid);

						myBloidMessage->objectId = newBloid.objectId;
						myBloidMessage->x = newBloid.x;
						myBloidMessage->y = newBloid.y;
						myBloidMessage->z = newBloid.z;
						myBloidMessage->direction = newBloid.direction;
						peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
					}
					myModifyMessage->typeId = ID_GAME_MESSAGE_3;
					peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					//this is really just a fix for coupling not spawning everyone
					myBloidMessage->typeID = ID_GAME_MESSAGE_1;

					for (int i = 0; i < bloids.size(); ++i)
					{
						
						myBloidMessage->objectId = bloids.at(i).objectId;
						myBloidMessage->x = bloids.at(i).x;
						myBloidMessage->y = bloids.at(i).y;
						myBloidMessage->z = bloids.at(i).z;
						myBloidMessage->direction = bloids.at(i).direction;
						peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
					}
				}
				//else 
				//{
				//	for (int i = 0; i < bloids.size(); ++i)
				//	{
				//		myBloidMessage->objectId = bloids.at(i).objectId;
				//		myBloidMessage->x = bloids.at(i).x;
				//		myBloidMessage->y = bloids.at(i).y;
				//		myBloidMessage->z = bloids.at(i).z;
				//		myBloidMessage->direction = bloids.at(i).direction;
				//		peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				//	}
				//	myModifyMessage->typeId = ID_GAME_MESSAGE_3;
				//	peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				//
				//	for (int i = 0; i < 4; ++i)
				//	{
				//		Bloid newBloid(bloids.size(), 0, 0, 0, -1, RakNet::GetTime());
				//		newBloid.setBloidDirection();
				//
				//		bloids.push_back(newBloid);
				//
				//		myBloidMessage->objectId = newBloid.objectId;
				//		myBloidMessage->x = newBloid.x;
				//		myBloidMessage->y = newBloid.y;
				//		myBloidMessage->z = newBloid.z;
				//		myBloidMessage->direction = newBloid.direction;
				//		peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				//	}
				//	myModifyMessage->typeId = ID_GAME_MESSAGE_3;
				//	peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				//}
				myBloidMessage->typeID = ID_GAME_MESSAGE_1;
				myModifyMessage->typeId = ID_GAME_MESSAGE_3;
				peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) 
				{
					printf("A client has disconnected.\n");
				}
				else 
				{
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer)
				{
					printf("A client lost the connection.\n");
				}
				else 
				{
					printf("Connection lost.\n");
				}
				break;
			case ID_GAME_MESSAGE_1:
				{
					customMessage *sentMessage = (customMessage *)packet->data;
					printf("%s\n", sentMessage->messageStr);
				}
			break;
			case ID_GAME_MESSAGE_4:
			{
				const BloidMessage *newBloidData = (BloidMessage *)packet->data;

				myBloidMessage->objectId = newBloidData->objectId;
				if (serverType == 2)
				{
					myBloidMessage->x = bloids.at(newBloidData->objectId).x;//(bloids.at(newBloidData->objectId).x * newBloidData->x) * 0.5;
					myBloidMessage->y = bloids.at(newBloidData->objectId).y;//(bloids.at(newBloidData->objectId).y * newBloidData->y) * 0.5;
					myBloidMessage->z = bloids.at(newBloidData->objectId).z;//(bloids.at(newBloidData->objectId).z * newBloidData->z) * 0.5;
				}
				else
				{
					myBloidMessage->x = newBloidData->x;
					myBloidMessage->y = newBloidData->y;
					myBloidMessage->z = newBloidData->z;
				}
				myBloidMessage->direction = bloids.at(newBloidData->objectId).direction;
				if (serverType == 2)
				{
					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
				else
				{
					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
				}
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
		if (serverType == 2)
		{
			if (timeCurr - timeServerUpdate >= SERVER_UPDATE)
			{
				timeServerUpdate = timeCurr;
				for (int i = 0; i < bloids.size(); ++i)
				{
					bloids.at(i).updateBloid(RakNet::GetTime());
					myBloidMessage->objectId = bloids.at(i).objectId;
					myBloidMessage->x = bloids.at(i).x;
					myBloidMessage->y = bloids.at(i).y;
					myBloidMessage->z = bloids.at(i).z;
					myBloidMessage->direction = bloids.at(i).direction;

					std::cout
						<< "ID: "
						<< myBloidMessage->objectId
						<< " | DIRECTION: "
						<< myBloidMessage->direction
						<< " \n";

					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}
			//spookiness at intervals of elapsed time
			if (timeCurr - timePrev >= 3000)
			{
				//this is where we update
				timePrev = timeCurr;
				srand(time(unsigned(NULL)));
				for (int i = 0; i < bloids.size(); ++i)
				{
					bloids.at(i).setBloidDirection();
					myBloidMessage->objectId = bloids.at(i).objectId;
					myBloidMessage->x = bloids.at(i).x;
					myBloidMessage->y = bloids.at(i).y;
					myBloidMessage->z = bloids.at(i).z;
					myBloidMessage->direction = bloids.at(i).direction;

					std::cout
						<< "ID: "
						<< myBloidMessage->objectId
						<< " | DIRECTION: "
						<< myBloidMessage->direction
						<< " \n";

					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}

			timeCurr = RakNet::GetTimeMS();
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
