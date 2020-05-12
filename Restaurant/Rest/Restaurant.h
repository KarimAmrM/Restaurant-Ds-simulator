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


#include<fstream>


#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;

	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	


	Queue<Order*> normalOrders;
	Queue<Order*> veganOrders;
	PriorityQueue<Order*> vipOrders;
	Queue<Order*> servingOrders;

	Queue<Cook*>availableCooks;
	Queue <Cook*>busyCooks;

	Queue<Order*> finishedOrders;

	ifstream  loadFile;
	ofstream saveFile;

	int normalCooks, veganCooks, vipCooks;// number of each cook

	int nCooks;
	int currentTimeStep;
	int promoteLimit;
	int urgentLimit;
	int numEvents;
	int nOrders;
	
	
	
	
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

	//
	// TODO: Add More Member Functions As Needed
	//



};

#endif