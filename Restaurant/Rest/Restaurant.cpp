#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"

using namespace std;


Restaurant::Restaurant() 
{
	pGUI = NULL;
	currentTimeStep = 1;
	nOrders = 0;
	nCooks = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Simulation();
		break;

	};

}

void Restaurant::LoadFromFile()
{

	loadFile.open("C:\\Users\\Fastoraaa\\Desktop\\y.txt");
	if (!loadFile.is_open()) {
		pGUI->PrintMessage("Load file doesn't exist in default directory! ");
		return;
	}
	 
		
	int normalSpeed, veganSpeed, vipSpeed;// speed of each cook
	
	int breakPerCook, normalBreaks, veganBreaks, vipBreaks;// orders before break and number of breaks
	char eventType, orderType;
	int eventTimeStep, eventId, orderSize;
	double orderPrice;

	loadFile >> normalCooks >> veganCooks >> vipCooks;
	loadFile >> normalSpeed >> veganSpeed >> vipSpeed;
	loadFile >> breakPerCook >> normalBreaks >> veganBreaks >> vipBreaks;

	nCooks = normalCooks + veganCooks + vipCooks;


			for (int i = 0; i < normalCooks; i++) 
			{
				
				//create cook 
				//fill cook list 
				//ids from 1 to normalCooks , id=i+1
				Cook* nrmCook = new Cook(i + 1, TYPE_NRM, normalSpeed, breakPerCook, normalBreaks);
				cooks.insert(nrmCook,i);
			}
		
			for (int i = 0; i < veganCooks; i++)
			{

				//fill cook list 
				//ids from normalCooks+1 to veganCooks, id=normalCooks+i+1
				Cook* vgnCook = new Cook(normalCooks+i + 1, TYPE_VGAN, veganSpeed, breakPerCook, veganBreaks);
				cooks.insert(vgnCook, i+normalCooks);
				
				

			}
			for (int i = 0; i < vipCooks; i++) 
			{

				//fill cook list 
				//ids from veganCooks+1 to vipCooks, id=veganCooks+1+i
				Cook* vipCook = new Cook(i + normalCooks +veganCooks+1, TYPE_VGAN, vipSpeed, breakPerCook, vipBreaks);
				cooks.insert(vipCook,i+veganCooks+normalCooks);
			}

			loadFile >> promoteAfter >> numEvents;
			

			for (int i = 0; i < numEvents; i++) { // loop responsible for reading events 
				//and queuing them
				loadFile >> eventType;
				
				switch (eventType) {
				
				case('R'): // Change constructor of arrivalEvent to fill order's info
					loadFile >>orderType>> eventTimeStep >> eventId >> orderSize >> orderPrice;
					nOrders++;
					if (orderType == 'N') 
					{
						
						Event* nEvent = new ArrivalEvent(eventTimeStep, eventId, TYPE_NRM,orderSize,orderPrice);
						addEvent(nEvent);
						
					}
					else if (orderType == 'G') 
					{
							
						Event* nEvent = new ArrivalEvent(eventTimeStep, eventId, TYPE_VGAN,orderSize,orderPrice);
						addEvent(nEvent);
						
					}
					else if (orderType == 'V') 
					{
					
						Event* nEvent = new ArrivalEvent(eventTimeStep, eventId, TYPE_VIP,orderSize,orderPrice);
						addEvent(nEvent);
						
					}

					
					break;
				case('X'):
					Event* nEvent;
					loadFile >> eventTimeStep >> eventId;
					
					
						//Cancellation event
						nEvent = new CancelationEvent(eventTimeStep, eventId);
						addEvent(nEvent);
						
						break;
		
				case('P'):
					loadFile >> eventTimeStep >> eventId >> orderPrice;
					

						//Promotion event
					break;
				default:
					break;
				}
				
			
			}
	
	
}


void Restaurant::addEvent( Event* nEvent)
{
	EventsQueue.enqueue(nEvent);
	return;

}

void Restaurant::assignToCook()
{
	Order* orderToServe;
	Cook* cookToPrepare;
	
	//We empty vip queue first
	while (vipOrders.peek(orderToServe))
	{

		for (int i = nCooks - 1; i >= normalCooks + veganCooks; i--) 
		{
			
			cookToPrepare = cooks.getEntry(i);
			if (cookToPrepare->GetFlag()) {

				//assign order to cook
				cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
				vipOrders.dequeue(orderToServe);
				servingOrders.enqueue(orderToServe);
				break;

			}
		
		
		}

		if (orderToServe->getStatus() == WAIT) 
		{
			
			for (int i = 0; i < normalCooks; i++) 
			{
				cookToPrepare = cooks.getEntry(i);

				if (cookToPrepare->GetFlag()) 
				{
				
					cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
					vipOrders.dequeue(orderToServe);
					servingOrders.enqueue(orderToServe);
					break;
				
				}
			
			}

		
		}
		if (orderToServe->getStatus() == WAIT) 
		{

			for (int i = normalCooks; i < normalCooks + veganCooks; i++) 
			{
				cookToPrepare = cooks.getEntry(i);
				if (cookToPrepare->GetFlag())
				{
				
					cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
					vipOrders.dequeue(orderToServe);
					servingOrders.enqueue(orderToServe);
					break;
				
				}



			}

		}

		if (orderToServe->getStatus() == WAIT) 
		{
		
			return;
        
		}

	}
	
	while (veganOrders.peekFront(orderToServe)) 
	{
	
	
		for (int i = normalCooks; i < normalCooks + veganCooks; i++)
		{
		
			cookToPrepare = cooks.getEntry(i);

			if (cookToPrepare->GetFlag())
			{
				cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
				veganOrders.dequeue(orderToServe);
				servingOrders.enqueue(orderToServe);
				break;
			}

		}

		if (orderToServe->getStatus() == WAIT) 
		{
		
		//no vegan cooks free
			break;
		
		}

	}
	while (normalOrders.peekFront(orderToServe)) 
	{
	
		for (int i = 0; i < normalCooks; i++) 
		{
			cookToPrepare = cooks.getEntry(i);
			
			if (cookToPrepare->GetFlag()) 
			{
				cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
				normalOrders.dequeue(orderToServe);
				servingOrders.enqueue(orderToServe);
				break;

			}
		
		}
	
		if (orderToServe->getStatus() == WAIT) 
		{
		
			for (int i = nCooks-1; i >= normalCooks + veganCooks;i--) 
			{
			
				cookToPrepare = cooks.getEntry(i);
			
				if (cookToPrepare->GetFlag())
				{
				
					cookToPrepare->AssignOrder(orderToServe, currentTimeStep);
					normalOrders.dequeue(orderToServe);
					servingOrders.enqueue(orderToServe);
					break;
				
				}

			}

		}
		if (orderToServe->getStatus() == WAIT) 
		{
		
			return;
		
		}

	}

}

void Restaurant::addOrder(Order* nOrder)
{
	if (nOrder->GetType()==TYPE_NRM) 
	{
	
		normalOrders.enqueue(nOrder);
		
	}
	else if (nOrder->GetType() == TYPE_VGAN)
	{
	
		veganOrders.enqueue(nOrder);
	

	}
	else if (nOrder->GetType() == TYPE_VIP) 
	{
	
		vipOrders.enqueue(nOrder,2);
		
	}

}

void Restaurant::cancelEvent(int ID)
{
	int count = 0;
	Order** O = normalOrders.toArray(count); //converting the normal orders cook queue to an arry to find the element with the matched id
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
	nOrders--;

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

void Restaurant::Simulation()
{
	LoadFromFile();
	while (!EventsQueue.isEmpty()|| !vipOrders.isEmpty() || !veganOrders.isEmpty() || !normalOrders.isEmpty()||!servingOrders.isEmpty())
	{
		ExecuteEvents(currentTimeStep);

		Order* O1, * O2, * O3;

		if (vipOrders.peek(O1))
		{
			servingOrders.enqueue(O1);
			O1->setStatus(SRV);
			vipOrders.dequeue(O1);
		}
		if (veganOrders.peekFront(O2))
		{
			servingOrders.enqueue(O2);
			O2->setStatus(SRV);
			veganOrders.dequeue(O2);
		}
		if (normalOrders.peekFront(O3))
		{
			servingOrders.enqueue(O3);
			O3->setStatus(SRV);
			normalOrders.dequeue(O3);
		}
		if (currentTimeStep % 5 == 0)
		{
			if (servingOrders.peekFront(O1))
			{
				finishedOrders.enqueue(O1);
				O1->setStatus(DONE);
				servingOrders.dequeue(O1);
			}
			if (servingOrders.peekFront(O2))
			{
				finishedOrders.enqueue(O2);
				O2->setStatus(DONE);
				servingOrders.dequeue(O2);
			}
			if (servingOrders.peekFront(O3))
			{
				finishedOrders.enqueue(O3);
				O3->setStatus(DONE);
				servingOrders.dequeue(O3);
			}
		}
		FillDrawingList();
		pGUI->UpdateInterface();
		
		pGUI->PrintMessage("Press to continue");
		pGUI->waitForClick();
		pGUI->PrintMessage("");
		pGUI->ResetDrawingList();
		
		currentTimeStep++;


	}


}





//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as lg ason there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);
	
	for (int i = 0; i < nCooks; i++) 
	{
		
		pGUI->AddToDrawingList(cooks.getEntry(i));
		
	}
	//Create 3 arrs to merge into one array
	// Sort array by arrival time
	int nVipOrders = 0 ;
	Order** vipOrdersArr = vipOrders.toArray(nVipOrders);
	int nNormalOrders = 0;
	Order** normalOrderArr = normalOrders.toArray(nNormalOrders);
	int nVeganOrders = 0;
	Order** veganOrderArr = veganOrders.toArray(nVeganOrders);
	int sum = nVeganOrders + nVipOrders + nNormalOrders;
	Order** orders = new Order * [sum];

	for (int i = 0; i < nVipOrders; i++)
	{
		orders[i] = vipOrdersArr[i];
	}
	for (int i = nVipOrders; i < nVipOrders + nNormalOrders; i++)
	{
		orders[i] = normalOrderArr[i];
	}
	for (int i = nVipOrders + nNormalOrders; i < sum; i++)
	{
		orders[i] = veganOrderArr[i];
	}
	int i, key, j;
	for (i = 1; i < sum; i++)
	{
		key = orders[i]->GetID();
		j = i - 1;

		while (j >= 0 && (orders[j]->GetID ()> key))
		{
			orders[j + 1] = orders[j];
			j = j - 1;
		}
		orders[i]->SetID(key);
	}
	for (int i = 0; i < sum; i++) 
	{
		pGUI->AddToDrawingList(orders[i]);
	}
	
	int servedOrd=0;
	Order** served = servingOrders.toArray(servedOrd);

	for (int i = 0; i < servedOrd; i++)
	{
		
		pGUI->AddToDrawingList(served[i]);
			
	}
	int finished=0;
	Order** finsihedArr = finishedOrders.toArray(finished);

	for (int i = 0; i < finished; i++) {
	
	
		pGUI->AddToDrawingList(finsihedArr[i]);
	
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
/*void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));
	LoadFromFile();

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	
	//Cook *pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);	
		//pC[i].setID(cID);
		//pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
	}	

		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////

*/
