
#include "Data.h"
#include <stdlib.h>     
#include <time.h>       

Bloid::Bloid()
{

}

Bloid::Bloid(int id, float nX, float nY, float nZ, int nDir, float nDT)
{
	objectId = id;
	x = nX;
	y = nY;
	z = nZ;

	direction = nDir;
	dt = nDT;
}

void Bloid::updateBloid(float timeChange)
{
	if (direction < 2 && direction >= 0)
	{
		if (0 == direction % 2)
			y += moveInc * ((timeChange - dt) * 0.001);
		else
			y -= moveInc * ((timeChange - dt) * 0.001);
	}
	else if (direction < 4)
	{
		if (0 == direction % 2)
			x += moveInc * ((timeChange - dt) * 0.001);
		else
			x -= moveInc * ((timeChange - dt) * 0.001);
	}
	else
	{
		if (0 == direction % 2)
			z += moveInc * ((timeChange - dt) * 0.001);
		else
			z -= moveInc * ((timeChange - dt) * 0.001);
	}
	dt = timeChange;
}


void Bloid::setBloidDirection()
{
	direction = rand() % 6;
}