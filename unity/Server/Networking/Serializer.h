//yes this is your code Dan.
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "RakNet/BitStream.h"


// serializable data
class Serializer
{
public:
	// serialize: defines how the data should be written to the bitstream
	//	bs: pointer to bitstream
	// return number of bytes written
	virtual int Serialize(RakNet::BitStream *bs) const = 0;

	// deserialize: defines how the data should be read from the bitstream
	//	bs: pointer to bitstream
	// return number of bytes read
	virtual int Deserialize(RakNet::BitStream *bs) = 0;

	// virtual dtor
	virtual ~Serializer() {};
};



#endif // !SERIALIZER_H

