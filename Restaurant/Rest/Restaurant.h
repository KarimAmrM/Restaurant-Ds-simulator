#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include"..\Events\CancelationEvent.h"
#include"../Data Structers/LinkedListStack.h"
#include"../Data Structers/LinkedList.h"
#include"../Data Structers\PriorityQueue.h"
#include"../Events/promoteEvent.h"

#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include<fstream>


#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;  
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	//orders Queues

	Queue<Order*> normalOrders;
	Queue<Order*> veganOrders;
	PriorityQueue<Order*> vipOrders;
	Queue<Order*> servingOrders;
	Queue<Order*> finishedOrders;

	//cooksQueues


	Queue<Cook*>availableVipCooks;
	Queue<Cook*>availableVeganCooks;
	Queue<Cook*>availableNormalCooks;
	Queue <Cook*>busyCooks;
	PriorityQueue < Cook*> onRestCooks;
	PriorityQueue<Cook*>onBreakCooks;
	


	ifstream  loadFile;
	ofstream saveFile;

	int normalCooks, veganCooks, vipCooks;// number of each cook

	int nCooks,numEvents;
	int currentTimeStep;
	int promoteLimit, numAutoPromoted, VIP_WT;
	int nOrders,numNormOrders,numVganOrders,numVipOreders,numWaitingVip;
	float injProb, numUrgentOrders;
	int numberAvailNormalCooks, numberAvailVipCooks, numberAvailVeganCooks;
	int numberBusyVipCooks, numberBusyVeganCooks, numberBusyNormalCooks;
	int numberInjured;
	int totalMoney;
	float totalWait, totalServe;
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void loadFromFile();
	void saveToFile();
	void addEvent(Event* nEvent );
	
	void addOrder(Order* nOrder);
	void cancelEvent(int ID);
	

	//functions for printing info on the status bar 
	int waitVipNumber();
	int waitNormalNumber();
	int waitVeganNumber();

	void FillDrawingList();
	bool assignToCook(Order*); //this function is called each timestep to assign orders to cook
	void Injury();
	void AssignUrgentOrder();
	void moveFromInservToFinished();
	void assigningOrders();


	void checkEndBreakOrRest();//checks on-break and in rest cooks if they finished their break/rest

	void Promote(int ID, double incMoney);
	void autoPromote();
	
	// TODO: Add More Member Functions As Needed
	//
	void InteractiveMode();
	void Step_by_StepMode();
	void SilentMode();
	void waitingTimeIncrementer();

};

#endif

