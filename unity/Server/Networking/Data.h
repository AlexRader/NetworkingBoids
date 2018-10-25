#ifndef DATA_H
#define DATA_H

struct Bloid
{
	const float moveInc = 5;
	int objectId;
	float x, y, z;
	int direction;

	Bloid();
	Bloid(int id, float nX, float nY, float nZ, int nDir);

	void updateBloid(float nX, float nY, float nZ);
	void setBloidDirection();
};

struct SentBloid
{
	int objectId;
	float x, y, z;
	int direction;
};

#endif // !DATA_H
