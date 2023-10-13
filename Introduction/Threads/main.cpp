#include<iostream>
#include<thread>
using std::cin;
using std::cout;
using std::endl;
using namespace std::literals::chrono_literals;

bool finish = false;

void Plus()
{
	while (!finish)
	{
		cout << "+ ";
		std::this_thread::sleep_for(1s);
	}
}
void Minus()
{
	while (!finish)
	{
		cout << "- ";
		std::this_thread::sleep_for(1s);
	}
}

void main()
{
	setlocale(LC_ALL, "");
	//Plus();
	//Minus();
	std::thread plus_thread(Plus);
	std::thread minus_thread(Minus);

	cin.get();	//ожидает нажатия 'Enter'.
	finish = true;

	plus_thread.join();
	minus_thread.join();
}