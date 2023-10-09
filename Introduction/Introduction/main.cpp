#include<iostream>
using namespace std;

struct Point
{
	double x;
	double y;
};

void main()
{
	setlocale(LC_ALL, "");
	//type name;
	int a;		//Объявление переменной 'a' типа 'int'
	Point A;	//Объявление переменной 'A' типа 'Point'
				//Создание объекта 'A' структуры 'Point'
				//Создание экземпляра 'A' структуры 'Point'
	A.x = 2;
	A.y = 3;
	cout << A.x << "\t" << A.y << endl;
}

/*
---------------------------------
.
->
---------------------------------
*/