#include<iostream>
#include<thread>
#include<chrono>
#include<Windows.h>
#include<conio.h>
#include<mutex>
using namespace std;
using namespace std::literals::chrono_literals;
enum Color
{
	Black, Blue, Green, Cyan, Red, Magenta, Brown,
	LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Spase = 32, Enter = 13, esc = 27 };
void setCursor(int x, int y)
{
	COORD myCoords = { x,y };//инициализируем передаваемыми значениями объект координат
	SetConsoleCursorPosition(hStdOut, myCoords);
}
#define MIN_TANK_VOLUME 35//объем бака
#define MAX_TANK_VOLUME 160
#define MIN_TANK_LEVEL 5//миним уровень топлива
#define MIN_ENGINE_CONSUMPTION 15//потребление на 100км
#define MAX_ENGINE_CONSUMPTION 30
#define MAX_SPEED_MAX 320//скорость
#define MAX_SPEED_MIN 140
#define MAX_ACCELERATION 40//ускорение
#define MIN_ACCELERATION 15
#define MIN_CONSUMPTION 0,003
class Tank
{
	const int VOLUME;//характеристика
	double fuel_level;//состояние
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const//вернуть уровень топлива
	{
		return fuel_level;
	}
	void fill(double fuel)//заправляем бак/бак получает топливо и прибавляем его к нашему уровню/если лить уже не куда уровень равен объему 
	{
		if (fuel < 0)return;
		if (fuel + this->fuel_level > VOLUME)this->fuel_level = VOLUME;
		else this->fuel_level += fuel;//если место в баке есть то продолжаем заправлять
	}
	double give_fuel(double amount)//расходуем топливо сколько попросил двиг/возвращаем уровень который остался в баке
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume) :VOLUME(
		volume< MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
		volume>MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
		volume), fuel_level(0)//инициализация в заголовке и проверка при помощи тернальника
	{
		/*	cout << "TConstructor\t" << this << endl;*/
			//if(volume<35)volume=35;
			//if(volume>160)volume=160;
			//this->VOLUME=volume;
	}
	~Tank()
	{
		/*cout << "TDestructor\t" << this << endl;*/
	}
	void Info()const
	{
		cout << "Volume:\t" << get_VOLUME() << " liters.\n";
		cout << "Fuel:\t" << get_fuel_level() << " liters.\n";
	}
	bool is_fill()//заправлен ли бак
	{
		if (get_fuel_level() > MIN_TANK_LEVEL && get_fuel_level() < VOLUME) return true;
		else return false;
	}
};
class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
public:
	Engine(double consumption) :CONSUMPTION(
		consumption<MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		consumption>MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
		consumption)
	{
		set_consuption_per_second();
		is_started = false;
		/*cout << "EConstructor\t" << this << endl;*/
	}
	~Engine()
	{
		/*cout << "EDestructor\t" << this << endl;*/
	}
	void set_consuption_per_second()
	{
		consumption_per_second = CONSUMPTION * 3e-5;
	}
	double get_consuption_per_second()const
	{
		return consumption_per_second;
	}
	double get_consuption()const
	{
		return CONSUMPTION;
	}
	void start()//либо приходит бул заправлен ли бак//по условию почему то на холостых бенз не тратиться
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()
	{
		return is_started;
	}
	void Info()const
	{
		cout << "Расход на 100 км.:  " << CONSUMPTION << " л." << endl;
		cout << "Расход за 1 секунду: " << consumption_per_second << " л." << endl;
		cout << "Двигатель " << (is_started ? "ON" : "OFF") << endl;//тернарник
	}
};
class Car
{
	Tank tank;
	Engine engine;
	int speed;
	const int MAX_SPEED;
	const int ACCELERATION;
	bool driver_inside;
	struct
	{
		std::thread panel_thread;
		std::thread idle_thread;
		std::thread friction_force;
	}threads;
public:
	Car(Tank tank, Engine engine, int max_speed = 250, int accelleration = 10) :
		tank(tank),
		engine(engine),
		speed(0),
		MAX_SPEED
		(
			max_speed< MAX_SPEED_MIN ? MAX_SPEED_MIN :
			max_speed>MAX_SPEED_MAX ? MAX_SPEED_MAX :
			max_speed),
		ACCELERATION
		(
			accelleration > MAX_ACCELERATION ? MAX_ACCELERATION :
			accelleration < MIN_ACCELERATION ? MIN_ACCELERATION :
			accelleration)
	{
		driver_inside = false;
		cout << "Your car is ready, press Enter to get in" << endl;
	}
	~Car()
	{
		cout << "Car :-(" << endl;
	}
	int get_speed()
	{
		return speed;
	}
	void driver_in()//открываем поток, когда водитель в машине для показа панели
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel, this);
	}
	void driver_out()
	{
		system("cls");
		driver_inside = false;
		if (threads.panel_thread.joinable())threads.panel_thread.join();
		cout << "Вы вышли! Чтобы заправится нажмите F" << endl;
		std::this_thread::sleep_for(1s);
	}
	void panel()
	{
		while (driver_inside)
		{
			system("cls");
			cout << "Вы в машине! Чтобы завести I/поехать W/тормозить S/выйти Enter" << endl;
			cout << "Уровень топлива: " << tank.get_fuel_level() << " литров" << endl;
			cout << "Двигатель: " << (engine.started() ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН") << endl;
			cout << "Скорость: " << speed << " км/ч" << endl;
			alarm();
			std::this_thread::sleep_for(1s);
		}
	}
	mutex mtx;
	void friction()
	{
		mtx.lock();
		while (speed > 0)
		{
			speed -= 1;
			if (speed < 1)speed = 0;
			std::this_thread::sleep_for(1s);
			cout << "Friction" << endl;
		}
		mtx.unlock();
	}
	//завести машину
	void idle_car()//при запуске расходуется только минимальное количество
	{
		while (engine.started() && tank.give_fuel(engine.get_consuption_per_second()))
		{
			if (tank.get_fuel_level() == 0) engine.stop();
			std::this_thread::sleep_for(1s);
		}
	}
	void move_car()
	{
		speed += ACCELERATION;
		if (speed > MAX_SPEED)speed = MAX_SPEED;
		tank.give_fuel(engine.get_consuption_per_second() * 4);//допустим, что на скорости расход увеличивается в 4 раза
		std::this_thread::sleep_for(1s);
		threads.friction_force = std::thread(&Car::friction, this);
		if (threads.friction_force.joinable())threads.friction_force.detach();
	}
	void brake_car()
	{
		speed -= ACCELERATION;
		if (speed < 1)speed = 0;
		tank.give_fuel(engine.get_consuption_per_second());
		std::this_thread::sleep_for(1s);
		threads.friction_force = std::thread(&Car::friction, this);
		if (threads.friction_force.joinable())threads.friction_force.detach();
	}
	void stop_car()
	{
		speed = 0;
		engine.stop();
		if (threads.idle_thread.joinable())threads.idle_thread.join();
	}
	void start_car()//открываем поток для старта машины, чтобы списывалось топливо, пока заведена
	{
		engine.start();
		threads.idle_thread = std::thread(&Car::idle_car, this);
	}
	void alarm()//мигалочка
	{
		if (!tank.is_fill()) cout << "Низкий уровень топлива!!!!!!!" << endl;
	}
	void car_brain()//управление машиной
	{
		char key = 0;
		do
		{
			if (_kbhit)//выполняетс выбранное действие, пока не нажмем другое
				key = _getch();//передаем управление для нажатия
			switch (key)
			{
			case Enter:if (!engine.started()) driver_inside ? driver_out() : driver_in();//выпрыгивать на ходу не будем)) не едем переделать!
				break;
			case 'F'://заправляем
			case 'f':
				if (driver_inside)//если машина не заведена и водитель вышел, заправляем
				{
					cout << "Нужно выйти из машины!" << endl;
					break;
				}
				double kol_fuel;
				cout << "Заправляю: "; cin >> kol_fuel;
				tank.fill(kol_fuel);
				break;
			case 'I'://зажигание
			case 'i':
				if (driver_inside)
				{
					engine.started() ? stop_car() : start_car();
				}
				break;
			case esc://выключаем и выходим
				stop_car();
				driver_out();
				cout << "Bye!" << endl;
				break;
			case 'W'://едем
			case 'w':
				if (driver_inside && engine.started())
				{
					move_car();
				}
				break;
			case 'S'://тормозим
			case 's':
				if (driver_inside && engine.started())
				{
					brake_car();
				}
				break;
			}
			if (tank.get_fuel_level() == 0) stop_car();
		} while (key != esc);
	}
	void Info()const
	{
		engine.Info();
		tank.Info();
		cout << "Max speed:    \t" << MAX_SPEED << " km/h\n";
		cout << "Acceleration:\t" << ACCELERATION << " km/h\n";
		cout << "Speed:\t\t\t" << speed << " km/h\n";
	}
};

#define TANK_CHECK
#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(20);//задефайнили бак
#endif // TANK_CHECK
#ifdef ENGINE_CHECK
	Engine engine(15);//задефайнили движок
#endif // ENGINE_CHECK
	Car car(tank, engine);
	car.car_brain();
}