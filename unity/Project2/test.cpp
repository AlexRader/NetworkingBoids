
#include <stdio.h>
#include <string.h>
#include <ctype.h> // needed for strlen
#include "RakNet/RakPeerInterface.h" //include changed to correct directory
#include "RakNet/MessageIdentifiers.h" //include changed to correct directory
#include "RakNet/BitStream.h" // include changed to correct directory
#include "RakNet/RakNetTypes.h"  // MessageID // include changed to correct directory
#include <iostream>


#define BoidInfo __declspec (dllexport)

extern "C"
{


	BoidInfo struct BoidData
	{
		struct BoidPosData
		{
			float posX, posY;
			float velX, velY;
			float accX, accY;
		    float moveSpeed;
			float rot;

		} posData;

		

		BoidData();
		BoidData(float nPosX, float nPosY, float nVelX, float nVelY, float nAccX, float nAccY, float speed, float rot);
		~BoidData();

		int Serialize();
		int Deserialize();

	};

	BoidInfo int random()
	{
		return rand();
	}

	BoidInfo BoidData::BoidData()
	{
		posData.posX = 0.0f;
		posData.posY = 0.0f;
		posData.velX = 0.0f;
		posData.velY = 0.0f;
		posData.accX = 0.0f;
		posData.accY = 0.0f;
		posData.moveSpeed = 0.0f;
		posData.rot = 0.0f;

	}

	BoidInfo BoidData::BoidData(float nPosX, float nPosY, float nVelX, float nVelY, float nAccX, float nAccY, float nSpeed, float nRot )
	{
		posData.posX = nPosX;
		posData.posY = nPosY;
		posData.velX = nVelX;
		posData.velY = nVelY;
		posData.accX = nAccX;
		posData.accY = nAccY;
		posData.moveSpeed = speed;
		posData.rot = nRot;

	}

	BoidInfo BoidData::~BoidData()
	{

	}


	BoidInfo BoidData::wander()
	{
		BoidPos.rot += random() * .25;
	}

	BoidInfo BoidData::Serialize(RakNet::BitStream *bs) const
	{
		if (bs)
		{
			unsigned int totalSz = 0;

			totalSz += sizeof(posData);
			bs->Write(posData);

			//done
			return totalSz;
		}
		return 0;
	}

	BoidInfo BoidData::Deserialize(RakNet::BitStream *bs)
	{
		if (bs)
		{
			unsigned int totalSz = 0;

			totalSz += sizeof(posData);
			bs->Read(posData);

			//done
			return totalSz;
		}

		return 0;
	}

}