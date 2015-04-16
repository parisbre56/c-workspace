#include "pos2D.hpp"
#include <cmath>

pos2D::pos2D(double xx, double yy):x(xx),y(yy)
{
	
}

pos2D::pos2D():x(0),y(0)
{
	
}

pos2D& pos2D::operator =(pos2D* a)
{
	x=a->x;
	y=a->y;
	return *this;
}

pos2D& pos2D::operator =(pos2D a)
{
	x=a.x;
	y=a.y;
	return *this;
}

bool pos2D::operator == (pos2D& a)
{
		return (a.x==x&&a.y==y);
}

bool pos2D::operator != (pos2D& a)
{
		return (a.x!=x&&a.y!=y);
}

double distance2D(pos2D a, pos2D b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

long int distance2Dint(pos2D a, pos2D b)
{
	return sqrt(((long int)a.x-(long int)b.x)*((long int)a.x-(long int)b.x)+((long int)a.y-(long int)b.y)*((long int)a.y-(long int)b.y));
}
