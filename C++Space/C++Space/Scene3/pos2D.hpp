#ifndef __X3IceSpos2D__
#define __X3IceSpos2D__

class pos2D
{
public:
	double x;
	double y;
	pos2D(double xx, double yy);
	pos2D();
	pos2D& operator = (pos2D a);
	pos2D& operator = (pos2D* a);
	bool operator == (pos2D& a);
	bool operator != (pos2D& a);
};

double distance2D (pos2D a, pos2D b);
//X and Y are treated as doubles
long int distance2Dint (pos2D a, pos2D b);
//X and Y are treated as long integers

#endif
