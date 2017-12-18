/*****************************************
- Quang Tran	 PSID: 1290921
- COSC 3360 Spring 2017
- Assignment 3 - PTHREAD program
*****************************************/
#include "Controller.h"
#include "InputList.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	string weight; string data;
	Controller controller;
	int maxWeight;
	weight = "";
	maxWeight = 0;
	InputList *list = new InputList();
	if (argc > 1)
	{
		stringstream ss(argv[1]);
		getline(ss, weight);
		maxWeight = stoi(weight.c_str());
		controller = Controller(maxWeight);
//		controller.readFile();
//		controller.execute();
	}
	else
		cout << "No max weight of bridge to be read!";
	while (getline(cin, data))
	{
		stringstream ss(data);
		string vehicle;
		int aTime, load, cTime;
		ss >> vehicle >> aTime >> load >> cTime;
		controller.storeFile(vehicle, aTime, load, cTime);
	}
	controller.execute();
//	system("pause");
	return 0;
}