#include "BloidSerializer.h"


BloidSerializer::BloidSerializer(const int id, const int dir, const float x, const float y, const float z)
{
	updateBloidSerializer(id, dir, x, y, z);
}

void BloidSerializer::updateBloidSerializer(const int id, const int dir, const float x, const float y, const float z)
{
	Bloid.objectId = id;
	Bloid.direction = dir;
	Bloid.x = x;
	Bloid.y = y;
	Bloid.z = z;
}


BloidSerializer::~BloidSerializer()
{
}

int BloidSerializer::Serialize(RakNet::BitStream *bs) const
{
	// ****TO-DO: 
	// stuff all data owned by this into array
	// do so in a way that it could be read back again
	if (bs)
	{
		unsigned int totalSz = 0;

		totalSz += sizeof(Bloid);
		bs->Write(Bloid);

		//done 
		return totalSz;
	}
	return 0;
}

int BloidSerializer::Deserialize(RakNet::BitStream *bs)
{
	// ****TO-DO: 
	// fundamentally the opposite of serialize
	// read things back in the same order they would be written
	// do allocations and deallocations as needed
	if (bs)
	{
		unsigned int totalSz = 0;

		totalSz += sizeof(Bloid);
		bs->Read(Bloid);
		//done 
		return totalSz;
	}
	return 0;
}
