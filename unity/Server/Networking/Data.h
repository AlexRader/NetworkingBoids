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

#ifndef DATA_H
#define DATA_H
#include "RakNet/GetTime.h"
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

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GAME_MESSAGE_2 = ID_USER_PACKET_ENUM + 2,
	ID_GAME_MESSAGE_3 = ID_USER_PACKET_ENUM + 3,
	ID_GAME_MESSAGE_4 = ID_USER_PACKET_ENUM + 4 // recieve client boids
};
// message to send to plugin
#pragma pack(push, 1)
struct BloidMessage
{
	char typeID = ID_GAME_MESSAGE_1;

	unsigned char useTimeStamp = ID_TIMESTAMP; // Assign ID_TIMESTAMP to this
	RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime() or some other method that returns a similar value

	int objectId;
	float x, y, z;
	int direction;
};
#pragma pack(pop)

//(Project2)  data structure used for managing server stored boids.
struct Bloid
{
	const float moveInc = 5; // (Project2) distance to move increment
	int objectId;
	float x, y, z;
	int direction;
	float dt; // (Project2) change in time

	Bloid();
	Bloid(int id, float nX, float nY, float nZ, int nDir, float nDT);

	void updateBloid(float timeChange);
	void setBloidDirection();
};

//(Project2) sent structure through the network.
struct BloidData
{
	int objectId;
	float x, y, z;
	int direction;
};

#endif // !DATA_H
