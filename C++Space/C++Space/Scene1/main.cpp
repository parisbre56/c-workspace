#include <iostream>
#include <cstdlib>

using namespace std;

//Enumerators
enum Colour {dark=0,light=1};

//Incomplete Classes
class Scene;
class Background;
class Lake;
class IceSkater;
class Mountains;
class Trees;
class Little_house;

//Classes
class Scene
{
	Background* background;
	Lake* lake;
	IceSkater* iceskater;
public:
	Scene(unsigned int L,unsigned int Dist);
	void plot(void);
	~Scene(void);
};

class Background
{
	Mountains* mountains;
	Trees* trees;
	Little_house* little_house;
public:
	Background(void);
	~Background(void);
};

class Lake
{
	unsigned int length;
	Colour colour;
public:
	Lake(unsigned int L)
	{
		length = L;
		colour = dark;
		cout<<"A lake appears!"<<endl;
	}
	void light_up(void)
	{
		colour = light;
		cout<<"Lights on!"<<endl;
	}
	void darken(void)
	{
		colour = dark;
		cout<<"Lights off!"<<endl;
	}
	void rotate(void)
	{
		cout<<"Lake rotated!"<<endl;
	}
	unsigned int getLength(void)
	{
		return length;
	}
	~Lake(void)
	{
		cout<<"A lake will dissapear!"<<endl;
	}
};

class IceSkater
{
	unsigned int distance;
	unsigned int position;
public:
	IceSkater(unsigned int Dist)
	{
		distance=Dist;
		position=0;
		cout<<"An IceSkater participates!"<<endl;
	}
	~IceSkater(void)
	{
		cout<<"An IceSkater will be discarted."<<endl;
	}
	void start(void)
	{
		cout<<"Here I am ..."<<endl;
	}
	void skate(long int distance)
	{
		position+=distance;
		cout<<"An IceSkater skates "<<(((distance/this->distance)*this->distance<distance)?((distance/this->distance)+1):(distance/this->distance))<<" times."<<endl;
	}
	void rotate(void)
	{
		cout<<"An IceSkater spins on position, "<<position<<" units away from the edge of the lake."<<endl;
	}
	void jump(void)
	{
		cout<<"An IceSkater jumps on position, "<<position<<" units away from the edge of the lake."<<endl;
	}
};

class Mountains
{
public:
	Mountains(void)
	{
		cout<<"Mountains were just created!"<<endl;
	}
	~Mountains(void)
	{
		cout<<"Mountains are about to be destroyed."<<endl;
	}
};

class Trees
{
public:
	Trees(void)
	{
		cout<<"Trees were just created!"<<endl;
	}
	~Trees(void)
	{
		cout<<"Trees are about to be destroyed."<<endl;
	}
};

class Little_house
{
public:
	Little_house(void)
	{
		cout<<"A little house was just created!"<<endl;
	}
	~Little_house(void)
	{
		cout<<"A little house is about to be destroyed."<<endl;
	}
};


//Class Member Functions
Scene::Scene(unsigned int L, unsigned int Dist)
{
	background = new Background();
	lake = new Lake(L);
	iceskater = new IceSkater(Dist);
	cout<<"Scene was just created!"<<endl;
}
void Scene::plot(void)
{
	/* Η λίμνη φωτίζεται.
	 * Η παγοδρόμος ξεκινά την επίδειξή της.
	 * Η παγοδρόμος γλιστρά εώς το κέντρο της λίμνης.
	 * Η παγοδρόμος περιστρέφεται.
	 * Η παγοδρόμος πηδά.
	 * Η παγοδρόμος γλιστρά εώς την άλλη άκρη της λίμνης.
	 * Η λίμνη περιστρέφεται.
	 * Η λίμνη σκοτεινιάζει.
	 */
	 lake->light_up();
	 iceskater->start();
	 iceskater->skate(lake->getLength()/2);
	 iceskater->rotate();
	 iceskater->jump();
	 iceskater->skate(lake->getLength()-lake->getLength()/2);
	 lake->rotate();
	 lake->darken();
}
Scene::~Scene(void)
{
	cout<<"Scene is about to be destroyed."<<endl;
	delete iceskater;
	delete lake;
	delete background;
}

Background::Background(void)
{
	mountains = new Mountains();
	trees = new Trees();
	little_house = new Little_house();
	cout<<"Background was just created!"<<endl;
}
Background::~Background(void)
{
	cout<<"Background is about to be destroyed."<<endl;
	delete little_house;
	delete trees;
	delete mountains;
}

//Main Function
int main(int argc, char **argv)
{
	unsigned int L = atoi(argv[1]);
	unsigned int Dist = atoi(argv[2]);
	Scene scene(L,Dist);
	scene.plot();
	return 0;
}
