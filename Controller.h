#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "InputList.h"
#include <pthread.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>

using namespace std;

class Controller
{
private:
	int maxWeight;
public:
	Controller();
	Controller(int weight);
	~Controller();
	void readFile();
	void storeFile(string vehicle, int aTime, int weight, int cTime);
	void execute();
};

