/*****************************************
- Quang Tran	 PSID: 1290921
- COSC 3360 Spring 2017
- Assignment 3 - PTHREAD program
*****************************************/
#include "Controller.h"
#define MAXCHILD 64

InputList *inputList = new InputList();
static pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
static int currentLoad = 0;
static int maxBridgeLoad = 0;
int listCount = 0;

Controller::Controller()
{
	maxWeight = 0;
}

Controller::~Controller()
{
}

Controller::Controller(int weight)
{
	maxWeight = weight;
	maxBridgeLoad = maxWeight;
}

// Function call when vehicle enter the bridge
int enterBridge(string vehicle, int weight)
{
	//	If load weight of vehicle exceed the maximum load that the bridge can handle
	//	then, the vehicle cannot pass the bridge
	if (weight > maxBridgeLoad){
		cout << "The vehicle " << vehicle << " has its load exceed the Max bridge load.";
		cout << "It cannot cross the bridge." << endl;
		return 1;
	}
	//	else, it will check whether the load of vehicle can exceed the maximum bridge's load if crossing bridge
	//	If yes, vehicle need to wait
	//	Otherwise, it can cross the bridge
	while (weight > (maxBridgeLoad - currentLoad)){
		pthread_cond_wait(&cond_var, &mylock);
		if (weight <= (maxBridgeLoad - currentLoad)) { pthread_mutex_unlock(&mylock); break; }
	}
	//	Print out vehicle start crossing bridge
	//	and the current bridge's load which include the load of crossing vehicle
	pthread_mutex_lock(&mylock);
	currentLoad += weight;
	cout << "The vehicle " << vehicle << "(load weight = " << weight << ")" << " start crossing the bridge" << endl;
	cout << "The current bridge load = " << currentLoad << endl;
	//	check whether the bridge can handle more vehicle to cross
	//	then signal
	if ((maxBridgeLoad - currentLoad) != 0) pthread_cond_signal(&cond_var);
	pthread_mutex_unlock(&mylock);
	return 0;
}

//	Function call to perform vehicle leave the bridge
int leaveBridge(string vehicle, int weight)
{
	//	Print out the leaving vehicle
	//	and the current bridge's load after vehicle left
	pthread_mutex_lock(&mylock);
	currentLoad -= weight;
	cout << "The vehicle " << vehicle << "(load weight = " << weight << ")" << " leave the bridge" << endl;
	cout << "The current bridge load = " << currentLoad << endl;
	pthread_cond_signal(&cond_var);
	pthread_mutex_unlock(&mylock);
}

//	Child thread function to perform the procedure
//	Vehicle arrive the bridge, enter the bridge, and leave the bridge
void *print_message(void *data)
{
	string *temp;
	temp = (string*)data;
	int numb = 0;
	int weight = stoi(temp[1].c_str());
	int cTime = stoi(temp[2].c_str());
	//	Print out vehicle has arrived bridge
	cout << "The vehicle " << temp[0] << "(load weight = " << temp[1] << ")" << " has arrived the bridge" << endl;
	cout << "The current load of bridge = " << currentLoad << endl;
	numb = enterBridge(temp[0], weight);	// call function to perform vehicle entering bridge
	//	when vehicle has entered and start crossing the bridge
	//	performing sleep in an amount of time to simulate the crossing time
	//	after that, vehicle leave the bridge
	if (numb != 1){
		for (int i = 0; i < cTime; i++){
	//		if ((maxBridgeLoad - currentLoad) != 0) pthread_cond_signal(&cond_var);
			sleep(1);
		}
		leaveBridge(temp[0], weight);
	}
	pthread_exit(NULL);
}
//	Function call to read the text file which not using IO redirection
//	This function can allow user to enter the file name
//	and check whether the file existed in designated folder
void Controller::readFile()
{
	ifstream infile;
	string data;
	string filename = "";
	bool check = false;
	do
	{
		cout << "Please enter your filename here: ";
		cin >> filename;
		infile.open(filename);
		if (!infile){
			cout << "Unable to open the file. Please check it again!" << endl;
			check = true;
		}
		else{
			check = false;
			inputList = new InputList();
			while (!infile.eof()){
				string data, licensePlate;
				int aTime, weight, cTime;
				stringstream ss;
				getline(infile, data);
				ss << data;
				ss >> licensePlate >> aTime >> weight >> cTime;
				inputList = inputList->setData(listCount,licensePlate, aTime, weight, cTime);
				listCount++;
			}
		}
	} while (check);
	cout << "List of vehicle data information in the text file:" << endl;
	cout << inputList->toString(inputList);
	maxBridgeLoad = maxWeight;
}
//	Function to store input text file into node list of data
void Controller::storeFile(string vehicle, int aTime, int weight, int cTime)
{
	inputList = inputList->setData(listCount, vehicle, aTime, weight, cTime);
	listCount++;
}

void Controller::execute()
{
	//	Create variables to be used
	pthread_t threads[MAXCHILD];	// create an array of thread child
	pthread_mutex_init(&mylock, NULL);
	data* temp;
	string *data;
	int rc;
	for (int i = 0; i < listCount; i++)
	{
		data = new string[3];
		temp = inputList->getListData(inputList, i);
		data[0] = temp->licensePlate;	
		data[1] = to_string(temp->loadWeight);		data[2] = to_string(temp->crossTime);
		for (int i = 0; i < temp->arrvTime; i++)
			sleep(1);
		rc = pthread_create(&threads[i], NULL, &print_message, (void*)data);
		if (rc){
			cout << "Unable to create thread" << endl;
			exit(1);
		}
	}
	for (int i = 0; i < listCount; i++)
		pthread_join(threads[i], NULL);
}