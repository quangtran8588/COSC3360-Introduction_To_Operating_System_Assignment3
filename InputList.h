#pragma once
#include <string>

using namespace std;

struct data
{
	int numbList;
	string licensePlate;
	int arrvTime;
	int loadWeight;
	int crossTime;
	data *next;
	data *prev;
	data() : licensePlate(""), arrvTime(0), loadWeight(0), crossTime(0){}
};

class InputList
{
private:
	bool empty;
	data* top;
	data* ptr;
public:
	InputList();
	~InputList();
	InputList* setData(int numb, string lPlate, int aTime, int weight, int cTime);
	data* getListData(InputList* inputList, int numb);
	string getLicensePlate(data *list);
	int getArrivalTime(data *list);
	int getLoadWeight(data *list);
	int getCrossingTime(data *list);
	void clean(InputList* dataList);
	string toString(InputList *list);
};