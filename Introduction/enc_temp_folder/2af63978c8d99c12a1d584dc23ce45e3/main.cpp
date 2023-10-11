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

#define MIN_ENGINE_CONSUMPTION	4
#define MAX_ENGINE_CONSUMPTION	30
class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
public:
	double get_CONSUMPTION()const
	{
		return CONSUMPTION;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	void set_consumption_per_second()
	{
		consumption_per_second = CONSUMPTION * 3e-5;//3 * 10 в -5й степени
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	Engine(double consumption) :
		CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		)
	{
		set_consumption_per_second();
		is_started = false;
		cout << "EConstructor:\t" << this << endl;
	}
	~Engine()
	{
		cout << "EDestructor:\t" << this << endl;
	}
	void info()const
	{
		cout << "Расход на 100 км.:  " << CONSUMPTION << " л." << endl;
		cout << "Расход за 1 секунду: " << consumption_per_second << " л." << endl;
		cout << "Двигатель " << (is_started ? "запущен" : "остановлен") << endl;
	}
};

#define MAX_SPEED_LOW_LIMIT		120
#define MAX_SPEED_HIGHT_LIMIT	400
#define MIN_ACCELLERATION		 15
#define MAX_ACCELLERATION		 40
class Car
{
	Engine engine;
	Tank tank;
	const int MAX_SPEED;
	const int ACCELERATION;
	int speed;
public:
	Car(Engine engine, Tank tank, int max_speed = 250, int accelleration = 10) :
		speed(0),
		tank(tank),		//Implicit copy constructor
		engine(engine),	//Implicit copy constructor
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGHT_LIMIT ? MAX_SPEED_HIGHT_LIMIT :
			max_speed
		),
		ACCELERATION
		(
			accelleration < MIN_ACCELLERATION ? MIN_ACCELLERATION :
			accelleration > MAX_ACCELLERATION ? MAX_ACCELLERATION :
			accelleration
		)
	{
		cout << "Your car is ready, press Enter to get in" << endl;
	}
	~Car()
	{
		cout << "Car is over :-(" << endl;
	}
	void info()const
	{
		engine.info();
		tank.info();
		cout << "Max speed:    \t" << MAX_SPEED << " km/h\n";
		cout << "Acceleration:\t" << ACCELERATION<< " km/h\n";
		cout << "Speed:\t\t\t" << speed << " km/h\n";
	}
};

//Deep copy - побитовое копирование
//Shallow copy - поверхностное копирование

//#define TANK_CHECK
//Директива #define (определить) создает макроопределение
//#define ENGINE_CHECK

void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK		//показывает начало блока кода
	//Если определено 'TANK_CHECK', то нижеследующий код будет виден компилятору:
	Tank tank(20);
	tank.info();
	while (true)
	{
		double fuel;
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CHECK	//показывает конец блока кода

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car car(10, 60);
	car.info();
}