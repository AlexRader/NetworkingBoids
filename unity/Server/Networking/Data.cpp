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
#include "Data.h"
#include <stdlib.h>     
#include <time.h>       

Bloid::Bloid()
{

}

//(Project2) cstor
Bloid::Bloid(int id, float nX, float nY, float nZ, int nDir, float nDT)
{
	objectId = id;
	x = nX;
	y = nY;
	z = nZ;

	direction = nDir;
	dt = nDT;
}

//(Project2) updates the position of bloids.
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

//(Project2) changes the direction of the bloid.
void Bloid::setBloidDirection()
{
	direction = rand() % 6;
}