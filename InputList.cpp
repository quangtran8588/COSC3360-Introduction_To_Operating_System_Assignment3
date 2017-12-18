/*****************************************
- Quang Tran	 PSID: 1290921
- COSC 3360 Spring 2017
- Assignment 3 - PTHREAD program
*****************************************/
#include "InputList.h"

InputList::InputList()
{
	empty = true;
	top = NULL;
	ptr = NULL;
}

InputList::~InputList()
{
}

InputList* InputList::setData(int numb, string lPlate, int aTime, int weight, int cTime)
{
	data *newList = new data();
	newList->numbList = numb;
	newList->licensePlate = lPlate;
	newList->arrvTime = aTime;
	newList->loadWeight = weight;
	newList->crossTime = cTime;
	if (empty) { newList->prev = NULL, newList->next = NULL; top = newList; ptr = newList; empty = false; }
	else
	{
		newList->prev = ptr;
		newList->next = NULL;
		ptr->next = newList;
		ptr = newList;
	}
	return this;
}

data* InputList::getListData(InputList* inputList, int numb)
{
	data *temp = inputList->top;
	do{
		if (temp->numbList == numb) return temp;
		else{
			if (temp->next != NULL) temp = temp->next;
			else break;
		}
	} while (1);
	return NULL;
}
string InputList::getLicensePlate(data *list)
{
	return list->licensePlate;
}
int InputList::getArrivalTime(data *list)
{
	return list->arrvTime;
}
int InputList::getLoadWeight(data *list)
{
	return list->loadWeight;
}
int InputList::getCrossingTime(data *list)
{
	return list->crossTime;
}

void InputList::clean(InputList* dataList)
{
	do
	{
		data* temp = dataList->top;
		if (temp->next != NULL)
		{
			temp->next->prev = NULL;
			dataList->top = temp->next;
			delete temp;
		}
		else
		{
			delete temp;
			empty = true;
			break;
		}
	} while (1);
}

string InputList::toString(InputList *dataList)
{
	string toString;
	data *temp = dataList->top;
	do
	{
		dataList->ptr = temp;
		toString += dataList->getLicensePlate(temp) + " " + to_string(dataList->getArrivalTime(temp)) + " " + to_string(dataList->getLoadWeight(temp)) + " " + to_string(dataList->getCrossingTime(temp)) + "\n";
		if (temp->next != NULL) temp = dataList->ptr->next;
		else break;
	} while (1);
	return toString;
}
