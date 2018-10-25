#include <stdio.h>
#include <string.h>
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include <iostream>


#define BloidInfo __declspec (dllexport)


//client plugin
extern "C"
{
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


	struct BloidMessage {

		char typeID;

		int objectId;
		float x, y, z;
		int direction;

	};

	struct BloidData {

		int objectId = -1;
		float x = 0, y = 0, z = 0;
		int direction = -1;

	};


	//connect
	BloidInfo void connectToServer(char* ip);

	//receive data
	BloidInfo BloidData receiveData();

	//send data back to server
	BloidInfo void sendData(int id, float x, float y, float z, int dir);

	//disconnect
	BloidInfo void kickRequest();

	
}