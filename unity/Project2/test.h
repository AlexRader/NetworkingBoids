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

#include <stdio.h>
#include <string.h>
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include <iostream>

// (Project2) this is to check for importing or exporting of files
#ifdef BLOID_EXPORT
#define BloidInfo __declspec (dllexport)
#else
#ifdef BLOID_IMPORT
#define BloidInfo __declspec (dllimport)

#else // BLOID_IMPORT

#define BloidInfo
#endif
#endif


//(Project2) client plugin
#ifdef __cplusplus
extern "C"
{
#endif
//(Project2)
enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1, // for packets sent usually 
	ID_GAME_MESSAGE_2 = ID_USER_PACKET_ENUM + 2, // for packets on join
	ID_GAME_MESSAGE_3 = ID_USER_PACKET_ENUM + 3, // cancel packet
	ID_GAME_MESSAGE_4 = ID_USER_PACKET_ENUM + 4	 // send client boids
};
#pragma pack(push, 1)
//(Project2) the sent bloid over network
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
//(Project2) the incoming data to be used by unity
#pragma pack(push, 1)
struct BloidData {


	float timeStamp = -1;
	int objectId = -1;
	float x = 0;
	float y = 0;
	float z = 0;
	int direction = -1;

};
#pragma pack(pop)

#pragma pack(push, 1)
struct customMessage
{
	GameMessages typeId = ID_GAME_MESSAGE_2;
	char messageStr[512];
};
#pragma pack(pop)


	//initialize (Project2)
	BloidInfo void raknetPeer();

	//connect(Project2)
	BloidInfo void connectToServer(char* ip);

	//receive data (Project2)
	BloidInfo BloidData receiveData();

	//send data back to server (Project2)
	BloidInfo void sendData(int id, float tStamp,float x, float y, float z, int dir);

	//disconnect (Project2)
	BloidInfo void kickRequest();

	//Test (Project2)
	BloidInfo BloidData Test();

	//intitialize bloids from server (Project2)
	BloidInfo BloidData InitialData();

#ifdef __cplusplus
}
#endif