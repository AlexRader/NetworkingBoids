#ifndef DATA_H
#define DATA_H
#include "RakNet/GetTime.h"

struct Bloid
{
	const float moveInc = 5;
	int objectId;
	float x, y, z;
	int direction;
	float dt; // change in time

	Bloid();
	Bloid(int id, float nX, float nY, float nZ, int nDir, float nDT);

	void updateBloid(float timeChange);
	void setBloidDirection();
};

struct BloidData
{
	int objectId;
	float x, y, z;
	int direction;
};

#endif // !DATA_H
