#ifndef _RadioList_
#define _RadioList_
#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;
class RadioList{

private:
	vector<string> myvector;
	int checked;
	COORD coord;
	int max;


public:
	RadioList();
	~RadioList();
	COORD getCoord();
	void setChecked(int s);
	int getChecked();
	void setCoord(COORD C);
	void display();
	


	/*
	AutoTaxi();
	void movePassenger(Point &s, Point &f, enum cell **city, AutoTaxi **G);
	AutoTaxi(const AutoTaxi &obj);
	AutoTaxi(Input a);
	void reduceHealth(int num);
	~AutoTaxi();
	bool GPS1(const Point &s, const Point &d, int *stepL, int *stepR, enum cell **city_1);
	bool GPS2(const Point &s, const Point &d, int *stepL, int *stepR, enum cell **city_1);
	void changePoint(int a, int b);
	Point getPoint() const;
	void goAhead(int);
	void goBack(int);
	void turnLeft(int);
	void turnRight(int);
	void pickUP();
	void takeOff();
	void printAutoTaxi() const;
	void drive(char *myfile); //take the file simulation and call the functions, and write this to file "output"
	int getID() const;
	void setID(int n);
	*/
};

#endif