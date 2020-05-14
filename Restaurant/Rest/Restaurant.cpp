#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"




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
	loadFile.open("1.txt");
	if (!loadFile.is_open()) {
		pGUI->PrintMessage("Load file doesn't exist in default directory! ");
		pGUI->waitForClick();
		return;
	}

	int normalSpeedMin,nomralSpeedMax, veganSpeedMax,veganSpeedMin ,vipSpeedMin,vipSpeedMax;// speed of each cook
	
	int orderBeforeBreak, normalBreaksMin,normalBreaksMax, veganBreaksMin,veganBreaksMax, vipBreaksMin,vipBreaksMax, restPeriod;// orders before break and number of breaks

	float injProb;
		

	char eventType, orderType;

	int eventTimeStep, eventId, orderSize;

	double orderPrice;

	loadFile >> normalCooks >> veganCooks >> vipCooks;
	loadFile >> normalSpeedMin>>nomralSpeedMax >> veganSpeedMin>>veganSpeedMax >> vipSpeedMin>>vipSpeedMax;
	loadFile >> orderBeforeBreak >> normalBreaksMin>>normalBreaksMax >> veganBreaksMin >>veganBreaksMax>> vipBreaksMin>>vipBreaksMax;
	loadFile >> injProb >> restPeriod;
	loadFile >> promoteLimit >> urgentLimit >> numEvents;


	nCooks = normalCooks + veganCooks + vipCooks;
	int cookBreak, cookSpeed;//to be generated randomly 
	cookBreak = 0;
	cookSpeed = 0;

			for (int i = 0; i < normalCooks; i++) 
			{
				//create cook 
				//fill cook list 
				//ids from 1 to normalCooks , id=i+1
				
				cookBreak = randomize(normalBreaksMax, normalBreaksMin);
				cookSpeed = randomize(normalBreaksMax, normalSpeedMin);

				Cook* nrmCook = new Cook(i + 1, TYPE_NRM, cookSpeed, orderBeforeBreak, cookBreak);
				availableCooks.enqueue(nrmCook);
			}
		
			for (int i = 0; i < veganCooks; i++)
			{
				//fill cook list 
				//ids from normalCooks+1 to veganCooks, id=normalCooks+i+1
				
				cookBreak = randomize(veganBreaksMax, veganBreaksMin);
				cookSpeed = randomize(veganSpeedMax, veganSpeedMin);

				Cook* vgnCook = new Cook(normalCooks + i + 1, TYPE_VGAN, cookSpeed, orderBeforeBreak, cookBreak);
				availableCooks.enqueue(vgnCook);

			}
			for (int i = 0; i < vipCooks; i++) 
			{

				//fill cook list 

				//ids from veganCooks+1 to vipCooks, id=veganCooks+1+i

				cookBreak = randomize(vipBreaksMax, vipBreaksMin);
				cookSpeed = randomize(vipSpeedMax, vipSpeedMin);

				Cook* vipCook = new Cook(i + normalCooks + veganCooks +1, TYPE_VIP, cookSpeed, orderBeforeBreak, cookBreak);
				availableCooks.enqueue(vipCook);
			}

			
			for (int i = 0; i < numEvents; i++) // loop responsible for reading events 
				//and queuing them
			{	
				loadFile >> eventType;
				
				switch (eventType) 
				{
				
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
				case('X'): // Cancellation event
					Event* nEvent;
					loadFile >> eventTimeStep >> eventId;

						//Cancellation event
						nEvent = new CancelationEvent(eventTimeStep, eventId);
						addEvent(nEvent);
						
						break;
		
				case('P'): // to be completed in phase 2
					int extraMoney;
					loadFile >> eventTimeStep >> eventId >> extraMoney;
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
	//continue simulation until there's no events nor any waiting/serving orders
	while (!EventsQueue.isEmpty() || !vipOrders.isEmpty() || !veganOrders.isEmpty() || !normalOrders.isEmpty() || !servingOrders.isEmpty())
	{
		ExecuteEvents(currentTimeStep);

		Order* O1, * O2, * O3;
		// check each order queue if there's an order move it to serving list
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
		// check every 5 time steps and move 3 orders to finished 
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

		//printing data info
		
		string currentTimePrinted = to_string(currentTimeStep);
		string vipWaitingOrdersPrinted = to_string(waitVipNumber());
		string normalWaitingOrdersPrinted = to_string(waitNormalNumber());
		string veganWaitingOrdersPrinted = to_string(waitVeganNumber());
		string vipCooksNumberPrinted = to_string(vipCooks);
		string normalCooksNumberPrinted = to_string(normalCooks);
		string veganCooksNumberPrinted = to_string(veganCooks);


		pGUI->PrintMessage("Current time step : " + currentTimePrinted + '\n' + "Current waiting VIP orders : " + vipWaitingOrdersPrinted + '\n' + "Current waiting vegan orders : " + veganWaitingOrdersPrinted + '\n'
			+ "Current waiting vegan orders : " + veganWaitingOrdersPrinted + '\n' + "Current available VIP cooks : " + vipCooksNumberPrinted + '\n'
			+ "Current available normal cooks : " + normalCooksNumberPrinted + '\n' + "Current available vegan cooks : " + veganCooksNumberPrinted + '\n' + "Click to continue.");


		pGUI->waitForClick();


		pGUI->ResetDrawingList();


		currentTimeStep++;


	}


}


//calling these functions when printing the info of the number of waiting orders . 

int Restaurant::waitVipNumber()
{
	int count=0;
	Order** waitVip = vipOrders.toArray(count);
	return count;
}

int Restaurant::waitNormalNumber()
{
	int count = 0;
	Order** waitNormal = normalOrders.toArray(count);
	return count;
}

int Restaurant::waitVeganNumber()
{
	int count=0;
	Order** waitVegan = veganOrders.toArray(count);
	return count;
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

	int avaialbeCooksCount = 0;
	Cook** availCooks = availableCooks.toArray(avaialbeCooksCount);
	for (int i = 0; i < avaialbeCooksCount; i++) 
	{
		
		pGUI->AddToDrawingList(availCooks[i]);
		
	}

	int busyCooksCount = 0;
	Cook** busyCooksarr = busyCooks.toArray(busyCooksCount);
	for (int i = 0; i < busyCooksCount; i++)
	{

		pGUI->AddToDrawingList(busyCooksarr[i]);

	}

	//Create 3 arrs to merge into one array
	
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
		orders[i] = normalOrderArr[i-nVipOrders];
	}

	for (int i = nVipOrders + nNormalOrders; i < sum; i++)
	{
		orders[i] = veganOrderArr[i-nVipOrders-nNormalOrders];
	}
	// Sort array by arrival time
	int i,  j;
	Order* key;

	for (i = 1; i < sum; i++)
	{
		key = orders[i];
		j = i - 1;

		while (j >= 0 && (orders[j]->GetArrTime ()> key->GetArrTime()))
		{
			orders[j + 1] = orders[j];
			j = j - 1;
		}
		orders[j + 1] = key;
	}
	// add waiting order list to drawing list
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