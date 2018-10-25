
#include "Data.h"
#include <stdlib.h>     
#include <time.h>       

Bloid::Bloid()
{

}

Bloid::Bloid(int id, float nX, float nY, float nZ, int nDir)
{
	objectId = id;
	x = nX;
	y = nY;
	z = nZ;

	direction = nDir;
}

void Bloid::updateBloid(float nX, float nY, float nZ)
{
	if (direction < 2)
	{
		if (0 == direction % 2)
			y += moveInc;
		else
			y -= moveInc;
	}
	else if (direction < 4)
	{
		if (0 == direction % 2)
			x += moveInc;
		else
			x -= moveInc;
	}
	else
	{
		if (0 == direction % 2)
			z += moveInc;
		else
			z -= moveInc;
	}
}


void Bloid::setBloidDirection()
{
	direction = rand() % 6;
}