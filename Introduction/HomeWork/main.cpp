#include<iostream>
#include<thread>
#include<chrono>
#include<Windows.h>
#include<conio.h>
#include <string>
#include<io.h>
#include<vector>
using namespace std;
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
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Space = 32, Enter = 13, esc = 27 };
void setCursor(int x, int y)
{
	COORD myCoords = { x,y };//�������������� ������������� ���������� ������ ���������
	SetConsoleCursorPosition(hStdOut, myCoords);
}
#define MIN_TANK_VOLUME 35//����� ����
#define MAX_TANK_VOLUME 160
#define MIN_TANK_LEVEL 5//����� ������� �������
#define MIN_ENGINE_CONSUMPTION 15//����������� �� 100��
#define MAX_ENGINE_CONSUMPTION 30
#define MAX_SPEED_MAX 320//��������
#define MAX_SPEED_MIN 140
#define MAX_ACCELERATION 40//���������
#define MIN_ACCELERATION 15
#define MIN_CONSUMPTION 0,003
class Tank
{
	const int VOLUME;//��������������
	double fuel_level;//���������
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const//������� ������� �������
	{
		return fuel_level;
	}
	void fill(double fuel)//���������� ���/��� �������� ������� � ���������� ��� � ������ ������/���� ���� ��� �� ���� ������� ����� ������ 
	{
		if (fuel < 0)return;
		if (fuel + this->fuel_level > VOLUME)this->fuel_level = VOLUME;
		else this->fuel_level += fuel;//���� ����� � ���� ���� �� ���������� ����������
	}
	double give_fuel(double amount)//��������� ������� ������� �������� ����/���������� ������� ������� ������� � ����
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume) :VOLUME(
		volume< MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
		volume>MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
		volume), fuel_level(0)//������������� � ��������� � �������� ��� ������ �����������
	{
		cout << "TConstructor\t" << this << endl;
		//if(volume<35)volume=35;
		//if(volume>160)volume=160;
		//this->VOLUME=volume;
	}
	~Tank()
	{
		cout << "TDestructor\t" << this << endl;
	}
	void Info()const
	{
		cout << "Volume:\t" << get_VOLUME() << " liters.\n";
		cout << "Fuel:\t" << get_fuel_level() << " liters.\n";
	}
	bool is_fill()//��������� �� ���
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
		cout << "EConstructor\t" << this << endl;
	}
	~Engine()
	{
		cout << "EDestructor\t" << this << endl;
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
	void start()//���� �������� ��� ��������� �� ���//�� ������� ������ �� �� �������� ���� �� ���������
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
		cout << "������ �� 100 ��.:  " << CONSUMPTION << " �." << endl;
		cout << "������ �� 1 �������: " << consumption_per_second << " �." << endl;
		cout << "��������� " << (is_started ? "ON" : "OFF") << endl;//���������
	}
};
class Car
{
	Tank tank;
	Engine engine;
	int speed;
	const int MAX_SPEED;
	const int ACCELERATION;
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
		cout << "Your car is ready, press Enter to get in" << endl;
	}
	~Car()
	{
		cout << "Car :-(" << endl;
	}
	void Info()const
	{
		engine.Info();
		tank.Info();
		cout << "Max speed:    \t" << MAX_SPEED << " km/h\n";
		cout << "Acceleration:\t" << ACCELERATION << " km/h\n";
		cout << "Speed:\t\t\t" << speed << " km/h\n";
	}
	void fill(double fuel)//��������� ������
	{
		tank.fill(fuel);
	}
	void set_speed(int speed)//������������� ��������
	{
		this->speed = speed;
	}
	int get_speed()
	{
		return speed;
	}
	//������� ������
	void lets_start(double amount)//��� ������� ����������� ������ ����������� ����������
	{
		amount = MIN_CONSUMPTION;
		if (tank.get_fuel_level() > 0)//�������� ������ ��� ����������� ������ �������
		{
			engine.start();
			tank.give_fuel(amount);
			alarm();
		}
		else engine.stop();
	}
	//����� �� ��� � ����
	void move_car(int speed)//�������� �������� (��� �� ��������� ���)
	{
		if (tank.get_fuel_level() > 0)//�������� ������ ��� ����������� ������ �������
		{
			set_speed(speed);
			tank.give_fuel(engine.get_consuption_per_second());
			alarm();//��������� ������� ���� ������ ������� ������� ���������
		}
		else engine.stop();
	}
	void stop_car()//������������� ������ � ��������� ���������(���� ��������� ��� ��������)
	{
		speed = 0;
		engine.stop();
	}
	bool started()
	{
		return engine.started();
	}
	void alarm()
	{
		if (!tank.is_fill()) cout << "������ ������� �������!!!!!!!" << endl;
	}
};

#define TANK_CHECK
#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(20);//����������� ���
#endif // TANK_CHECK
#ifdef ENGINE_CHECK
	Engine engine(15);//����������� ������
#endif // ENGINE_CHECK
	Car car(tank, engine);
	int key, kol_fuel = 0;
	do//��� �� �������, � �������� ������ �������
	{
		system("cls");
		car.Info();
		cout << "��������� ������ ������ Spase" << endl;
		cout << "������� ������ ������� Enter" << endl;
		cout << "����� ������� Up" << endl;
		cout << "������������ ������� Esc" << endl;
		key = _getch();
		switch (key)
		{
		case Up:
			if (car.started())//���� ��������� ������� ��������
			{
				do
				{
					car.Info();
					Sleep(200);
					system("cls");
					car.move_car(5);//����� �� ��� � �������� ����� �� ��������
				} while (!_kbhit());//���� ���� �� �������)) ��������� ����� �������
			}
			break;
		case Space:
			if (!car.started())//���� ������ �� ��������, ����������
			{
				cout << "���������: ";
				cin >> kol_fuel;
				cout << " ������" << endl;
				car.fill(kol_fuel);
			}
			break;
		case Enter:
			car.lets_start(0);//�������
			break;
		case esc:
			car.stop_car();//���������
			break;
		}
	} while (true);
}