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

#include<stdlib.h>
#include<time.h>

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

	PriorityQueue<Cook*>onBreakCooks;
	Queue<Cook*>availableVipCooks;
	Queue<Cook*>availableVeganCooks;
	Queue<Cook*>availableNormalCooks;
	Queue<Cook*>availableCooks; //to be removed 
	Queue <Cook*>busyCooks;
	Queue<Cook*>injuredCooks;


	ifstream  loadFile;
	ofstream saveFile;

	int normalCooks, veganCooks, vipCooks;// number of each cook

	int nCooks;
	int currentTimeStep;
	int promoteLimit;
	int urgentLimit;
	int numEvents;
	int nOrders;
	
	int numberAvailNormalCooks, numberAvailVipCooks, numberAvailVeganCooks;
	int numberBusyVipCooks, numberBusyVeganCooks, numberBusyNormalCooks;
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void LoadFromFile();
	void addEvent(Event* nEvent );
	
	void addOrder(Order* nOrder);
	void cancelEvent(int ID);
	void Simulation();

	//functions for printing info on the status bar 
	int waitVipNumber();
	int waitNormalNumber();
	int waitVeganNumber();

	void FillDrawingList();
	bool assignToCook(Order*); //this function is called each timestep to assign orders to cook
	

	// TODO: Add More Member Functions As Needed
	//



};

#endif