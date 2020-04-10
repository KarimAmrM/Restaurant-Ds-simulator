#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
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

	LinkedList<Cook*> cooks;

	Queue<Order*> normalOrders;
	Queue<Order*> veganOrders;
	PriorityQueue<Order*> vipOrders;

	LinkedListStack<Order*> finishedOrders;

	ifstream  loadFile;
	ofstream saveFile;

	int nCooks;
	int currentTimeStep;
	int promoteAfter;
	int numEvents;
	int nOrders;
	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
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
	void assignToCook();
	void addOrder(Order* nOrder);
	
	
	void cancelEvent(int ID)  
	{
		int count = 0;
		Order** O= normalOrders.toArray(count); //converting the normal orders cook queue to an arry to find the element with the matched id
		for (int i = 0; i < count; i++)
		{
			if (O[i]->GetID() == ID)
			{
				int pos = i;
				for (int i = 0; i < count; i++) //removing the element 
				{
					if (i >= pos)
					{
						O[i] = O[i + 1];
					}
				}
				count--;
			}
		}

		while (!normalOrders.isEmpty()) //emptyting the queue to refill it again without the order with the canceled id 
		{
			Order* dummy;
			normalOrders.dequeue(dummy);
		}


		for (int i = 0; i < count; i++)//refilling the queue again 
		{
			normalOrders.enqueue(O[i]);
		}
	}


	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//


/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif