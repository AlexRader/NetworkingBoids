
//Base code taken from 
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample1.html
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample2.html
//http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html
//http://www.cplusplus.com/reference/cstdlib/strtoul/
//
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

// message to send to plugin
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
	unsigned int serverType; //(Project2) sets if the server is push, shared or coupled
	unsigned short serverPort = 1111; //(Project2) set this so it is relatively easy to connect between plugin and server

	char str[512];
	
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer = true;
	RakNet::Packet *packet;

	str[0] = '0';// this will need to be initialized for later down the line


	//get the maximum number of clients 
	printf("Please enter the max clients you wish to have\n");
	fgets(str, 512, stdin);

	maxClients = (unsigned short) strtoul(str, NULL, 0);

	//(Project2) sets the servertype and assures that it is correct.
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
	//We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(maxClients); //modified with local variables

	timePrev = RakNet::GetTimeMS();
	timeServerUpdate = RakNet::GetTimeMS();

	//(Project2) initialize a few boids for pushing.
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
			case ID_NEW_INCOMING_CONNECTION: //(Project2) when a new person joins there is a few things to do
			{
				printf("A connection is incoming.\n");
				myBloidMessage->typeID = ID_GAME_MESSAGE_2; //(Project2) this is known to the plugin as a setting up bloids message
				//(Project2) Relatively easy just spawn the bloids in the unity plugin for the push data
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
				//(Project2)both share and coupled have similar setups (probably why coupled doesnt work)
				else if (serverType >= 2)
				{
					//(Project2)spawns boids already known by the server
					for (int i = 0; i < bloids.size(); ++i)
					{
						myBloidMessage->objectId = bloids.at(i).objectId;
						myBloidMessage->x = bloids.at(i).x;
						myBloidMessage->y = bloids.at(i).y;
						myBloidMessage->z = bloids.at(i).z;
						myBloidMessage->direction = bloids.at(i).direction;
						peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
					myModifyMessage->typeId = ID_GAME_MESSAGE_3; // (Project2)known as an end of bloids list to the plugin
					peer->Send((char*)myModifyMessage, sizeof(customMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					//(Project2)spawns the locally controlled bloids and adds them to the list of bloids.
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
					//(Project2) this is really just a fix for coupling not spawning everyone (well it works but not quite)
					if (serverType == 3)
					{
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
				}

				myBloidMessage->typeID = ID_GAME_MESSAGE_1; //(Project2) switch to running message sending 
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
			case ID_GAME_MESSAGE_4: //(Project2) so this is basically the sent from client message for share and couple
			{
				const BloidMessage *newBloidData = (BloidMessage *)packet->data;
				//(Project2) testing for making sure data was being bassed through correctly
				//char str1[10];
				//sprintf(str1, "%d", newBloidData->objectId);
				//printf("%s\n", str1);

				//printf("%s\n", newBloidData->objectId);
				myBloidMessage->objectId = newBloidData->objectId;
				if (serverType == 2) //(Project2) if share we need to use a different method of movement than coupled
				{
					myBloidMessage->x = (bloids.at(newBloidData->objectId).x * newBloidData->x) * 0.5;
					myBloidMessage->y = (bloids.at(newBloidData->objectId).y * newBloidData->y) * 0.5;
					myBloidMessage->z = (bloids.at(newBloidData->objectId).z * newBloidData->z) * 0.5;
				}
				else //(Project2) coupled movement Note: one way coupled was explained to me was objects are tracked
					//on client and sent through the server like a player controlled character
				{
					myBloidMessage->x = newBloidData->x;
					myBloidMessage->y = newBloidData->y;
					myBloidMessage->z = newBloidData->z;
				}
				myBloidMessage->direction = bloids.at(newBloidData->objectId).direction;
				if (serverType == 2) //(Project2) send the update to everyone on server
					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				else // (Project2) send to everyone but client who sent the info
					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
		if (serverType <= 2) // (Project2) the update for data push and share
		{
			// (Project2) limits the server update rate, so the server isnt overloaded with messages.
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

					//(Project2) testing for id and direction
					//std::cout
					//	<< "ID: "
					//	<< myBloidMessage->objectId
					//	<< " | DIRECTION: "
					//	<< myBloidMessage->direction
					//	<< " \n";

					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}
			//(Project2) switch direction of travel.
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

					//std::cout
					//	<< "ID: "
					//	<< myBloidMessage->objectId
					//	<< " | DIRECTION: "
					//	<< myBloidMessage->direction
					//	<< " \n";

					peer->Send((char*)myBloidMessage, sizeof(BloidMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}

			timeCurr = RakNet::GetTimeMS();
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
