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
