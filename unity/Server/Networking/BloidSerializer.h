//Still using your code as a base

#ifndef BLOID_SERIALIZER_H
#define BLOID_SERIALIZER_H

#include "Serializer.h"

class BloidSerializer : public Serializer
{
	//basic bloid that should be sent out
	struct 
	{
		int objectId;
		float x, y, z;
		int direction;
	} Bloid;

public:
	BloidSerializer(const int id, const int dir, const float x, const float y, const float z);
	void updateBloidSerializer(const int id, const int dir, const float x, const float y, const float z);
	~BloidSerializer();


	// override serialize from base class
	virtual int Serialize(RakNet::BitStream *bs) const;

	// override deserialize from base class
	virtual int Deserialize(RakNet::BitStream *bs);

};
#endif // !BLOID_SERIALIZER_H