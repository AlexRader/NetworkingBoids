
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
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include "RakNet/GetTime.h"

#include "Proj2Events.h"
#include "Proj2Queue.h"
#include "Data.h"

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GAME_MESSAGE_2 = ID_USER_PACKET_ENUM + 2,
	ID_GAME_MESSAGE_3 = ID_USER_PACKET_ENUM + 3
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
	unsigned char typeID;

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
	std::vector <Bloid> bloids;
	std::vector <RakNet::RakNetGUID> IDsToUpdate;

	float timePrev, timeCurr;
	customMessage myModifyMessage[1];
	BloidMessage myBloidMessage[1];

	unsigned int maxClients; 
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

	//modified with local variables
	SocketDescriptor sd(serverPort, 0);
	peer->Startup(maxClients, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(maxClients); //modified with local variables

	for (int bl = 0; bl < 32; ++bl)
	{
		Bloid newBloid(bl, 0, 0, 0, -1);
		bloids.push_back(newBloid);
	}

	timePrev = RakNet::GetTimeMS();

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
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");



				strncpy(myModifyMessage->messageStr, "Hello world", sizeof(myModifyMessage->messageStr));
				myModifyMessage->typeId = ID_GAME_MESSAGE_1;
				peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");

				strncpy(myModifyMessage->messageStr, "testing testing 1 2 3", sizeof(myModifyMessage->messageStr));
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
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
		//Lab 3 timed server event messages for connected players (note host is excluded as we dont want evereyone losing connection)
		if (isServer)
		{
			//spookiness at intervals of elapsed time
			if (timeCurr - timePrev  >= 3000)
			{
				//myModifyMessage->typeId = ID_GAME_MESSAGE_2;
				//strncpy(myModifyMessage->messageStr, "Kitty is pretty spoopy! \n", sizeof(myModifyMessage->messageStr));
				//peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

				//this is where we update
				timePrev = timeCurr;
				for (int i = 0; i < bloids.size(); ++i)
				{
					bloids.at(i).setBloidDirection();
					myBloidMessage->objectId = bloids.at(i).objectId;
					myBloidMessage->x = bloids.at(i).x;
					myBloidMessage->y = bloids.at(i).y;
					myBloidMessage->z = bloids.at(i).z;
					myBloidMessage->direction = bloids.at(i).direction;
					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				
					std::cout << "SENT BOID INFO:"
						<< "ID: "
					<<	myBloidMessage->objectId
					<<	"DIRECTION: "
					<<	myBloidMessage->direction
					<<	" \n \n";
				
				
				
				}
				

			}
		}

		timeCurr = RakNet::GetTimeMS();
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
