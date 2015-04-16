#include <cstdlib>
#include <ctime>

class myclass
{
	int b;
	int c;
public:
	const unsigned long a;
	
	myclass(unsigned long);
};

myclass::myclass(unsigned long b):a(b)
{
	this->b=rand()/2;
	c=rand()/2+b;
}