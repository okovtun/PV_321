#include<iostream>
using namespace std;

#define MIN_TANK_VOLUME	 35
#define MAX_TANK_VOLUME	160
class Tank
{
	const int VOLUME;	//Характеристика объекта
	double fuel_level;	//Состояние объекта
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		if (fuel + this->fuel_level > VOLUME)this->fuel_level = VOLUME;
		else this->fuel_level += fuel;
	}
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume) :VOLUME(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME : 
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME : 
			volume), 
		fuel_level(0)
	{
		/*
		if (volume < 35)volume = 35;
		if (volume > 160)volume = 160;
		this->VOLUME = volume;
		*/
		cout << "TConstructor:\t" << this << endl;
	}
	~Tank()
	{
		cout << "TDestructor:\t" << this << endl;
	}

	void info()const
	{
		cout << "Volume:\t" << get_VOLUME() << " liters.\n";
		cout << "Fuel:\t" << get_fuel_level() << " liters.\n";
	}
};

void main()
{
	setlocale(LC_ALL, "");
	Tank tank(20);
	tank.info();
}