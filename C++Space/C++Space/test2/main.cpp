#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "class.hpp"

using namespace std;

int main(int argc, char **argv)
{
	srand(time(NULL));
	myclass* lass;
	lass=new myclass(rand()%2);
	cout<<lass->a<<endl;
	return 0;
}
