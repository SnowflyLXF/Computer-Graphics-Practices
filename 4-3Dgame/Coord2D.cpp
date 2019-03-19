#include "Coord2D.h"


Coord2D::Coord2D()
{
	i = 0;
	j = 0;
}

Coord2D::Coord2D(int a, int b)
{
	i = a;
	j = b;
}


Coord2D::~Coord2D()
{
}

void Coord2D::setI(int i)
{
	i = i;
}

void Coord2D::setJ(int j)
{
	j = j;
}

int Coord2D::getI()
{
	return i;
}

int Coord2D::getJ()
{
	return j;
}

void Coord2D::print()
{
	printf("%i , %i\n", i, j);
}
