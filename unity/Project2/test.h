#include <stdio.h>
#include <string.h>
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include <iostream>

#ifdef BLOID_EXPORT
#define BloidInfo __declspec (dllexport)
#else
#ifdef BLOID_IMPORT
#define BloidInfo __declspec (dllimport)

#else // BLOID_IMPORT

#define BloidInfo
#endif
#endif


//client plugin
#ifdef __cplusplus
extern "C"
{
#endif
//	struct BloidMessage {
//
//		unsigned char typeID;
//
//		int objectId;
//		float x, y, z;
//		int direction;
//
////		BloidMessage();
////		BloidMessage(char type, int id, float nX, float nY, float nZ, int nDir);
//
//	};

#pragma pack(push, 1)
struct BloidMessage {

	char typeID;

	int objectId;
	float x, y, z;
	int direction;

};
#pragma pack(pop)

#pragma pack(push, 1)
struct BloidData {

	int objectId = -1;
	float x = 0;
	float y = 0;
	float z = 0;
	int direction = -1;

};
#pragma pack(pop)

	//initialize 
	BloidInfo void raknetPeer();

	//connect
	BloidInfo void connectToServer(char* ip);

	//receive data
	BloidInfo BloidData receiveData();

	//send data back to server
	BloidInfo void sendData(int id, float x, float y, float z, int dir);

	//disconnect
	BloidInfo void kickRequest();

	//Test
	BloidInfo BloidData Test();

#ifdef __cplusplus
}
#endif