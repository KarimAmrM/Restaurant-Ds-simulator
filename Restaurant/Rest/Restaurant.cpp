#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"


int randomize(int max, int min) // generates random number between range{min to max}
{

	srand(time(NULL));
	return  (rand() % (max - min + 1)) + min;

}

Restaurant::Restaurant() 
{
	pGUI = NULL;
	currentTimeStep = 0;
	nOrders = 0;
	nCooks = 0;
	numberInjured = 0;
	totalMoney = 0;
	numNormOrders = 0;
	numVganOrders = 0;
	numVipOreders = 0;
	numUrgentOrders = 0;
	totalWait = 0;
	totalServe = 0;
	numAutoPromoted = 0;
	numberBusyNormalCooks = 0;
	numberBusyVipCooks = 0;
	numberBusyVeganCooks = 0;
	numWaitingVip = 0;
	

}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	
	PROG_MODE	mode = pGUI->getGUIMode();
	
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		
		InteractiveMode();
		break;
	case MODE_STEP:
		
		Step_by_StepMode();
		break;
	case MODE_SLNT:

		SilentMode();
		break;
	}

}

void Restaurant::loadFromFile()
{
	loadFile.open(loadFileName+".txt");
	if (!loadFile.is_open()) {
		pGUI->PrintMessage("Load file doesn't exist in default directory! ");
		pGUI->waitForClick();
		return;
	}

	float normalSpeedMin,normalSpeedMax, veganSpeedMax,veganSpeedMin ,vipSpeedMin,vipSpeedMax;// speed of each cook
	
	int orderBeforeBreak, normalBreaksMin,normalBreaksMax, veganBreaksMin,veganBreaksMax, vipBreaksMin,vipBreaksMax, restPeriod;// orders before break and number of breaks

	//float injProb;
		

	char eventType, orderType;

	int eventTimeStep, eventId, orderSize;

	double orderPrice;

	loadFile >> normalCooks >> veganCooks >> vipCooks;
	numberAvailNormalCooks = normalCooks;
	numberAvailVipCooks = vipCooks;
	numberAvailVeganCooks = veganCooks;
	loadFile >> normalSpeedMin>>normalSpeedMax >> veganSpeedMin>>veganSpeedMax >> vipSpeedMin>>vipSpeedMax;
	loadFile >> orderBeforeBreak >> normalBreaksMin>>normalBreaksMax >> veganBreaksMin >>veganBreaksMax>> vipBreaksMin>>vipBreaksMax;
	loadFile >> injProb >> restPeriod;
	loadFile >> promoteLimit >> VIP_WT >> numEvents;


	nCooks = normalCooks + veganCooks + vipCooks;
	int cookBreak;
	float cookSpeed;//to be generated randomly 
	cookBreak = 0;
	cookSpeed = 0;

			for (int i = 0; i < normalCooks; i++) 
			{
				//create cook 
				//fill cook list 
				//ids from 1 to normalCooks , id=i+1
				
				cookBreak = randomize(normalBreaksMax, normalBreaksMin);
				cookSpeed = normalSpeedMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (normalSpeedMax - normalSpeedMin)));
			
				
				Cook* nrmCook = new Cook(i + 1, TYPE_NRM, cookSpeed, orderBeforeBreak,cookBreak,restPeriod);
				availableNormalCooks.enqueue(nrmCook);
				cout << "Cook " << nrmCook->GetID() << " speed = " << cookSpeed << " and his break = " << cookBreak << endl;
			}
		
			for (int i = 0; i < veganCooks; i++)
			{
				//fill cook list 
				//ids from normalCooks+1 to veganCooks, id=normalCooks+i+1
				
			cookBreak = randomize(veganBreaksMax, veganBreaksMin);
				
				cookSpeed = veganSpeedMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (veganSpeedMax - veganSpeedMin)));

				Cook* vgnCook = new Cook(normalCooks + i + 1, TYPE_VGAN, cookSpeed, orderBeforeBreak, cookBreak,restPeriod);
				availableVeganCooks.enqueue(vgnCook);
				cout << "Cook " << vgnCook->GetID() << " speed = " << cookSpeed << " and his break = " << cookBreak << endl;
			}
			for (int i = 0; i < vipCooks; i++) 
			{

				//fill cook list 

				//ids from veganCooks+1 to vipCooks, id=veganCooks+1+i

				cookBreak = randomize(vipBreaksMax, vipBreaksMin);
				cookSpeed = vipSpeedMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (vipSpeedMax -vipSpeedMin)));

				Cook* vipCook = new Cook(i + normalCooks + veganCooks +1, TYPE_VIP, cookSpeed, orderBeforeBreak, cookBreak,restPeriod);
				availableVipCooks.enqueue(vipCook);
				cout << "Cook " << vipCook->GetID() << " speed = " << cookSpeed << " and his break = " << cookBreak << endl;
			}

			
			for (int i = 0; i < numEvents; i++) // loop responsible for reading events 
				//and queuing them
			{	
				loadFile >> eventType;
				
				switch (eventType) 
				{
				case('R'): // Change constructor of arrivalEvent to fill order's info
					loadFile >>orderType>> eventTimeStep >> eventId >> orderSize >> orderPrice;
					
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
		
				case('P'): // promote event
					int extraMoney;
					loadFile >> eventTimeStep >> eventId >> extraMoney;
					nEvent = new promoteEvent(eventTimeStep, eventId, extraMoney);
					addEvent(nEvent);

					break;
				default:
					break;
				}
			}
}

void Restaurant::saveToFile() {

	Order** finishedOrdersArr;
	Order* order;
	int numFinishedOrders = 0;
	
	finishedOrdersArr=finishedOrders.toArray(numFinishedOrders);



	// modified insertion sort to sort order by arrival time if they have the same finish time
	int i, j;
	Order* key;
	for (i = 1; i < numFinishedOrders; i++)
	{
			key = finishedOrdersArr[i];
			j = i - 1;

			//only sorts if orders have same finish time
		while (j >= 0 && finishedOrdersArr[j]->GetArrTime() > key->GetArrTime() && finishedOrdersArr[j]->GetFinishTime() == key->GetFinishTime())
		{
				finishedOrdersArr[j + 1] = finishedOrdersArr[j];
				j = j - 1;
		}
			finishedOrdersArr[j + 1] = key;
	}
	



	
	saveFile.open("saveFile.txt");
	if (!saveFile)
	{ 
		pGUI->PrintMessage("out file failed!!error");
		pGUI->waitForClick();
		return;
	}
	
	
	

	saveFile << "FT " << "   " << " ID" << "   " << " AT" << "   " << " WT" << "    " << " ST" <<endl;
	
	for (int i = 0; i < numFinishedOrders; i++) 
	{
		saveFile << finishedOrdersArr[i]->GetFinishTime() << "     " << finishedOrdersArr[i]->GetID() << "     " << finishedOrdersArr[i]->GetArrTime()
			<< "     " << finishedOrdersArr[i]->getWaitTime() << "     " << finishedOrdersArr[i]->GetServTime() << endl;
	}
	saveFile << "...................................................." << endl;

	saveFile <<"Orders: " <<nOrders << "  " << "[Normal:" << numNormOrders << ", " << "Vegan:" << numVganOrders << ", " << "VIP:" << numVipOreders << "]" << endl;

	saveFile <<"Cooks: " <<nCooks << "  " << "[Normal:" << normalCooks << ", " << "Vegan:" << veganCooks << ", " << "VIP:" << vipCooks <<", "<<"injured: "<<numberInjured <<"]" << endl;

	saveFile << "Average wait= " << totalWait / nOrders << ", " << "Average serve= " << totalServe / nOrders << endl;

	saveFile << "UrgentOrders: " << numUrgentOrders << ",  " << "Auto-promoted: " << setprecision(2)<<fixed<<(numAutoPromoted / numNormOrders) * 100;


	saveFile.close();



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
		cout << "Order " << nOrder->GetID() << " added to normal orders queue"<< "Order is auto promoted to vip at " <<currentTimeStep+promoteLimit <<endl;
	}
	else if (nOrder->GetType() == TYPE_VGAN)
	{
		veganOrders.enqueue(nOrder);
		cout << "Order " << nOrder->GetID() << " added to vegan orders queue" << endl;
	}
	else if (nOrder->GetType() == TYPE_VIP) 
	{
		double money = nOrder->GetTotalMoney();
		int arrivalTime = nOrder->GetArrTime();
		int size = nOrder->GetOrdSize();
		numWaitingVip++;
		vipOrders.enqueue(nOrder,exp((money/size*arrivalTime))/arrivalTime);
		cout << "Order " << nOrder->GetID() << " added to vip orders queue" <<"Must be promoted to urgent after : " <<currentTimeStep+promoteLimit << endl;
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
			cout << "order " << O[i]->GetID() << " is cancelled"<<endl;
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
	string currentTimePrinted;
	string vipWaitingOrdersPrinted;
	string normalWaitingOrdersPrinted;
	string veganWaitingOrdersPrinted;
	string vipCooksNumberPrinted;
	string normalCooksNumberPrinted;
	string veganCooksNumberPrinted;
	string orderAssignedToCook;
	string totalOrders;
	string numNormalOrders, numVeganOrders, numVipOrders;

	//available vip cooks
	int avaialbeVipCooksCount = 0;
	Cook** availableVipCooksArr = availableVipCooks.toArray(avaialbeVipCooksCount);
	for (int i = 0; i < avaialbeVipCooksCount; i++)
	{
		pGUI->AddToDrawingList(availableVipCooksArr[i]);
	}
	//available normal cooks
	int avaialbeNormCooksCount = 0;
	Cook** availableNormCooksArr = availableNormalCooks.toArray(avaialbeNormCooksCount);
	for (int i = 0; i < avaialbeNormCooksCount; i++) 
	{
		pGUI->AddToDrawingList(availableNormCooksArr[i]);
	}
	//available vegan cooks
	int avaialbeVeganCooksCount = 0;
	Cook** availableVeganCooksArr = availableVeganCooks.toArray(avaialbeVeganCooksCount);
	for (int i = 0; i < avaialbeVeganCooksCount; i++)
	{
		pGUI->AddToDrawingList(availableVeganCooksArr[i]);
	}
	// cooks serving 
	int busyCooksCount = 0;
	Cook** busyCooksArr = busyCooks.toArray(busyCooksCount);
	for (int i = 0; i < busyCooksCount; i++)
	{
		if (busyCooksArr[i]->GetCurrentOrder()->getOrderAssignedAt() == currentTimeStep - 1) 
		{
		
			string cookType;
			string cookId;
			string orderType;
			string orderId;

			switch (busyCooksArr[i]->GetType()) 
			{
			case TYPE_VIP:
				cookType = "V";
				break;
			case TYPE_NRM:
				cookType = "N";
				break;
			case TYPE_VGAN:
				cookType = "G";
				break;
			}
			cookId = to_string(busyCooksArr[i]->GetID());
			switch (busyCooksArr[i]->GetCurrentOrder()->GetType())
			{
			case TYPE_VIP:
				orderType = "V";
				break;
			case TYPE_NRM:
				orderType = "N";
				break;
			case TYPE_VGAN:
				orderType = "G";
				break;
			}
			orderId = to_string(busyCooksArr[i]->GetCurrentOrder()->GetID());
			orderAssignedToCook += cookType + cookId + orderType + orderId + "   ";

		
		}
		

	}

	
	//waiting orders
	int nVipOrders = 0 ;
	Order** vipOrdersArr = vipOrders.toArray(nVipOrders);
	int nNormalOrders = 0;
	Order** normalOrderArr = normalOrders.toArray(nNormalOrders);
	int nVeganOrders = 0;
	Order** veganOrderArr = veganOrders.toArray(nVeganOrders);
	
	

	for (int i = 0; i < nVipOrders; i++)
	{
		pGUI->AddToDrawingList(vipOrdersArr[i]);
	}

	for (int i = 0; i < nVeganOrders; i++)
	{
		pGUI->AddToDrawingList(veganOrderArr[i]);
	}

	for (int i = 0; i < nNormalOrders; i++)
	{
		pGUI->AddToDrawingList(normalOrderArr[i]);
	}
	
	// Sort array by arrival time
	/*int i,  j;
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
	}*/
	// add waiting order list to drawing list
	
	// in service orders
	int servedOrd=0;
	Order** served = servingOrders.toArray(servedOrd);
	for (int i = 0; i < servedOrd; i++)
	{
		//cout << served[i]->GetFinishTime() << "   ";
		pGUI->AddToDrawingList(served[i]);	
	}
	//cout << endl;
	// done orders
	int finished=0;
	Order** finsihedArr = finishedOrders.toArray(finished);
	for (int i = 0; i < finished; i++) 
	{
		pGUI->AddToDrawingList(finsihedArr[i]);
	}


	 currentTimePrinted = to_string(currentTimeStep);
	 vipWaitingOrdersPrinted = to_string(waitVipNumber());
	 normalWaitingOrdersPrinted = to_string(waitNormalNumber());
	 veganWaitingOrdersPrinted = to_string(waitVeganNumber());
	 vipCooksNumberPrinted = to_string(numberAvailVipCooks);
	 normalCooksNumberPrinted = to_string(numberAvailNormalCooks);
	 veganCooksNumberPrinted = to_string(numberAvailVeganCooks);
	 numNormalOrders = to_string(numNormOrders);
	 numVipOrders = to_string(numVipOreders);
	 numVeganOrders = to_string(numVganOrders);
	
	 if (orderAssignedToCook == "") 
	 {
		 orderAssignedToCook = "No orders assigned last time step!.";
	 }
	  
	 pGUI->PrintMessage("Current time step : " + currentTimePrinted + "                                         " + orderAssignedToCook + '\n'
		 + "Current waiting VIP orders : " + vipWaitingOrdersPrinted + "                         " + "Current Done VIP orders: " + numVipOrders + '\n'
		 + "Current waiting vegan orders : " + veganWaitingOrdersPrinted +"                    " +"Current Done vegan orders: "+numVeganOrders + '\n'
		+ "Current waiting normal orders : " + normalWaitingOrdersPrinted +"                   "+"Current Done normal orders: " +numNormalOrders+'\n'
		+ "Current available VIP cooks : " + vipCooksNumberPrinted + '\n'
		+ "Current available normal cooks : " + normalCooksNumberPrinted + '\n' 
		+ "Current available vegan cooks : " + veganCooksNumberPrinted + '\n' 
		+ "Click to continue."+'\n');

	orderAssignedToCook = "";
	pGUI->UpdateInterface();
	pGUI->ResetDrawingList();
}

bool Restaurant::assignToCook(Order*orderToAssigned)
{
	Cook* cookToAssign;		//A cook ptr to hold the cook dequed from whatever queue. 
	
	if (orderToAssigned->GetType() == TYPE_VIP) //checking if the order is vip
	{
		if (!availableVipCooks.isEmpty())								//checking the vip cooks queue first
		{
			  availableVipCooks.dequeue(cookToAssign);                     //remove this cook from the queue of available cooks
			  cookToAssign->AssignOrder(orderToAssigned, currentTimeStep); //assigns the order to the cook
			  busyCooks.enqueue(cookToAssign);								//enqueueing the cook to the busy cooks queue
			  servingOrders.enqueue(orderToAssigned);						//adding the order to the inservice queue of cooks
			  totalMoney = totalMoney + orderToAssigned->GetTotalMoney();	//adding the money of the order to the total money 
			  numberAvailVipCooks--;										//decrementing the available number of vip cooks
			  numberBusyVipCooks++;											//incremting the Number of busy cooks
			  numWaitingVip--;
			  cout << "order " << orderToAssigned->GetID() << " is assigned to " << cookToAssign->GetID()<<" order finish time is "<<orderToAssigned->GetFinishTime() <<endl;
			  return true;
		}
		else if (!availableNormalCooks.isEmpty())   //if there are no available vip then check for normal cooks then repating the same procedures 
		{
			availableNormalCooks.dequeue(cookToAssign);					
			cookToAssign->AssignOrder(orderToAssigned, currentTimeStep);
			busyCooks.enqueue(cookToAssign);							
			servingOrders.enqueue(orderToAssigned);					
			totalMoney = totalMoney + orderToAssigned->GetTotalMoney();
			numberAvailNormalCooks--;
			numberBusyNormalCooks++;
			numWaitingVip--;
			cout << "order " << orderToAssigned->GetID() << " is assigned to " << cookToAssign->GetID() << " order finish time is " << orderToAssigned->GetFinishTime() << endl;
			return true;
		}
		else if (!availableVeganCooks.isEmpty())  //if both vip queues and normal queues are empty then check for vegan cooks then repateing the same procedures 
		{
			availableVeganCooks.dequeue(cookToAssign);
			cookToAssign->AssignOrder(orderToAssigned, currentTimeStep);
			busyCooks.enqueue(cookToAssign);
			servingOrders.enqueue(orderToAssigned);
			totalMoney = totalMoney + orderToAssigned->GetTotalMoney();
			numberAvailVeganCooks--;
			numberBusyVeganCooks++;
			numWaitingVip--;
			cout << "order " << orderToAssigned->GetID() << " is assigned to " << cookToAssign->GetID() << " order finish time is " << orderToAssigned->GetFinishTime() << endl;
			return true;
		}
		return false;
	}
	else if (orderToAssigned->GetType() == TYPE_NRM) //if the order is normal 
	{
		if (!availableNormalCooks.isEmpty())   //checking for normal available cooks first then repating the same procudeures
		{
			availableNormalCooks.dequeue(cookToAssign);
			cookToAssign->AssignOrder(orderToAssigned, currentTimeStep);
			busyCooks.enqueue(cookToAssign);
			servingOrders.enqueue(orderToAssigned);
			totalMoney = totalMoney + orderToAssigned->GetTotalMoney();
			numberAvailNormalCooks--;
			numberBusyNormalCooks++;
			cout << "order " << orderToAssigned->GetID() << " is assigned to cook " << cookToAssign->GetID() << " and order finish time is " << orderToAssigned->GetFinishTime() << endl;
			return true;
		}
		else if (!availableVipCooks.isEmpty()) //checking for vip available cooks first then repeating the same steps
		{
			availableVipCooks.dequeue(cookToAssign);
			cookToAssign->AssignOrder(orderToAssigned, currentTimeStep);
			busyCooks.enqueue(cookToAssign);
			servingOrders.enqueue(orderToAssigned);
			totalMoney = totalMoney + orderToAssigned->GetTotalMoney();
			numberAvailVipCooks--;
			numberBusyVipCooks++;
			cout << "order " << orderToAssigned->GetID() << " is assigned to cook " << cookToAssign->GetID() << " and order finish time is " << orderToAssigned->GetFinishTime() << endl;
			return true;
		}
		return false;
	}
	else if (orderToAssigned->GetType() == TYPE_VGAN) 
	{
		if (!availableVeganCooks.isEmpty())  //one if condition only because vegan orders can only be perfomed by vegan cooks then repating the same procedures
		{
			availableVeganCooks.dequeue(cookToAssign);
			cookToAssign->AssignOrder(orderToAssigned, currentTimeStep);
			busyCooks.enqueue(cookToAssign);
			servingOrders.enqueue(orderToAssigned);
			totalMoney = totalMoney + orderToAssigned->GetTotalMoney();
			numberAvailVeganCooks--;
			numberBusyVeganCooks++;
			cout << "order " << orderToAssigned->GetID() << " is assigned to cook " << cookToAssign->GetID() << " and order finish time is " << orderToAssigned->GetFinishTime() << endl;
			return true;
		}
		return false;
	}
}

void Restaurant::Injury()
{
	if (busyCooks.isEmpty())
	{
		return;        //the function is for busy cooks only
	}
	else
	{
		
		float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);   //generating a random number
		if (p <= injProb)
		{
			Cook* InjCook=nullptr; //a pointer to hold the injured cook
			busyCooks.peekFront(InjCook); 
			if (InjCook->isInjured())	//if this cook is already injured
				return;
			InjCook->setinjured(true); //changing the cook's status to injured
			int passedTime = currentTimeStep - InjCook->GetCurrentOrder()->getOrderAssignedAt();//calculating the time passed from the serving time to the current time step
			int doneDishes = passedTime*InjCook->GetSpeed(); //calculating the number of done dishes until the current time step
			int newSpeed = InjCook->GetSpeed() / 2; //decrement the cook's speed to half its value
			InjCook->setSpeed(newSpeed); // setting the new speed
			int remainingDishes = InjCook->GetCurrentOrder()->GetOrdSize() - doneDishes; //calculating the number of the remaining dishes
			int newCookingTime = ceil(remainingDishes / float(newSpeed)); //calculating the new cooking time of the cook's order
			int newFinishTime = currentTimeStep +newCookingTime; //calculating the new finish time of the cook's order
			InjCook->GetCurrentOrder()->SetFinishTime(newFinishTime); // setting the new finish time of the cook's order
			numberInjured++;
			cout << "cook " << InjCook->GetID() << " injured and his order new finish time is " << InjCook->GetCurrentOrder()->GetFinishTime() << endl;
		}

	}

}

/*void Restaurant::AssignUrgentOrder()
{
	Order* UrgentOrder = nullptr; //a pointer to hold the urgent order
	vipOrders.peek(UrgentOrder); 
	if (UrgentOrder == nullptr)
	{
		return; //if there is no vip order
	}
	int WaitingTime = currentTimeStep - UrgentOrder->GetArrTime();//calculating the waiting time of the order
	while (WaitingTime >= VIP_WT) //to handle the case of many orders at the same time step
	{
		numUrgentOrders++;
		UrgentOrder->setUrgent(true);
		bool assigned=assignToCook(UrgentOrder); //a boolean to check either the order is assigned to a cook or not 
		if (!assigned) //in case there is no free cook
		{
			if (!onBreakCooks.isEmpty()) //searching for on break cook to assign the order to
			{
				Cook* UrgentCook = nullptr; //a pointer to hold the cook
				onBreakCooks.dequeue(UrgentCook); //dequeuing the cook from the on break cooks queue
				busyCooks.enqueue(UrgentCook); //enqueuing the cook in the busy cooks queue
				servingOrders.enqueue(UrgentOrder); //enqueuing the order in the serving orders queue
				UrgentCook->AssignOrder(UrgentOrder,currentTimeStep); //assigning the order to the cook
				assigned = true; //setting the order to assigned
			}
			else if (!onRestCooks.isEmpty()) //if there is no on break cook, we start to search for on rest cook
			{
				Cook* UrgentCook = nullptr; //a pointer to hold the cook
				onRestCooks.dequeue(UrgentCook); //dequeuing the cook from the on rest cooks queue
				busyCooks.enqueue(UrgentCook); //enqueuing the cook in the busy cooks queue
				servingOrders.enqueue(UrgentOrder); //enqueuing the order in the serving orders queue
				UrgentCook->AssignOrder(UrgentOrder, currentTimeStep); //assigning the order to the cook
				assigned = true;  //setting the order to assigned
			}

		}
		if (!assigned)
		{
			return; //in case the order isn't assigned, there is no need to check the next order
		}
		vipOrders.peek(UrgentOrder);
		if (!UrgentOrder)
		{
			return; //if there is no next order
		}
		else
		{
			WaitingTime = currentTimeStep - UrgentOrder->GetArrTime(); //if there is next order, calculate the waiting time of this order
		}
	}


}*/



void Restaurant::AssignUrgentOrder()
{
	if (vipOrders.isEmpty())
	{
		return; //if there is no vip order
	}

	Order* UrgentOrder = nullptr; //a pointer to hold the urgent order
	int count = 0;
	Order** VipOrders = vipOrders.toArray(count); //converting the vip orders queue into an array to search for urgent orders

	int waitingTime = 0;

	for (int i = 0; i < count; i++)
	{
		waitingTime = VipOrders[i]->getWaitTime(); // calculating the waiting time of the order
		if (waitingTime>=VIP_WT)
		{
		    VipOrders[i]->setUrgent(true);
		    numUrgentOrders++;
			cout << "Order " << UrgentOrder->GetID() << " is urgent order ";
			UrgentOrder = VipOrders[i];
			bool assigned = assignToCook(UrgentOrder); //a boolean to check either the order is assigned to a cook or not
			if (!assigned) //in case there is no free cook
			{
				if (!onBreakCooks.isEmpty()) //searching for on break cook to assign the order to
				{
					Cook* UrgentCook = nullptr; //a pointer to hold the cook
					onBreakCooks.dequeue(UrgentCook); //dequeuing the cook from the on break cooks queue
					busyCooks.enqueue(UrgentCook); //enqueuing the cook in the busy cooks queue
					servingOrders.enqueue(UrgentOrder); //enqueuing the order in the serving orders queue
					UrgentCook->AssignOrder(UrgentOrder, currentTimeStep); //assigning the order to the cook
					assigned = true; //setting the order to assigned
					cout << "and assigned to " << UrgentCook->GetID()<<" whose is in break"<<endl;
				}
				else if (!onRestCooks.isEmpty()) //if there is no on break cook, we start to search for on rest cook
				{
					Cook* UrgentCook = nullptr; //a pointer to hold the cook
					onRestCooks.dequeue(UrgentCook); //dequeuing the cook from the on rest cooks queue
					busyCooks.enqueue(UrgentCook); //enqueuing the cook in the busy cooks queue
					servingOrders.enqueue(UrgentOrder); //enqueuing the order in the serving orders queue
					UrgentCook->AssignOrder(UrgentOrder, currentTimeStep); //assigning the order to the cook
					assigned = true;  //setting the order to assigned
					cout << "and assigned to " << UrgentCook->GetID() << " whose is in Rest"<<endl;
				}

			}
			if (!assigned)
			{
				return;   //in case the order isn't assigned, there is no need to check the next order
			}
			else
			{
				int position = i;
				for (int i = position; i < count; i++)
				{
					VipOrders[i] = VipOrders[i + 1];
				}
				count--;
			}
		}
	}
	while (!vipOrders.isEmpty()) //emptyting the queue to refill it again after removing urgent orders
	{
		Order* dummy;
		vipOrders.dequeue(dummy);
	}


	for (int i = 0; i < count; i++)//refilling the queue again 
	{
		double money = VipOrders[i]->GetTotalMoney();
		int arrivalTime = VipOrders[i]->GetArrTime();
		int size = VipOrders[i]->GetOrdSize();
		numWaitingVip++;
		vipOrders.enqueue(VipOrders[i], exp((money / size * arrivalTime)) / arrivalTime);
	}

}

void Restaurant::moveFromInservToFinished()  
{
	if (busyCooks.isEmpty() )  //if there are no cooks serving any orders return
		return;

	Cook* c;  //a place holder to hold the dequeued cooks from either queues
	Order* finishedOrder; //to move the orders between the queues
	int sum = numberBusyNormalCooks + numberBusyVeganCooks + numberBusyVipCooks;
	if (!busyCooks.isEmpty())    //first we check the queue of the busy cooks that are serving orders without being injured
	{
		for (int i = 0; i <sum; i++)
		{
			busyCooks.peekFront(c);
			if (c->GetCurrentOrder()->GetFinishTime() <= currentTimeStep) //checking if the top cook has an order to be finished at this current time step
			{
				busyCooks.dequeue(c);					//if the cook is serving an order that has the same time as the current timestep then remove it from the queue of busy cooks
				finishedOrder = c->GetCurrentOrder();	//setting the order of the cook to the finsihed order
				c->removeOrder(currentTimeStep);						//removing the order from the cook
				servingOrders.dequeue(finishedOrder);	//move the order from inservice list to the finished orders list
				finishedOrders.enqueue(finishedOrder);	//we add the order to the finished orders queue

				cout << "Order " << finishedOrder->GetID() << " is finished ";
				nOrders++;
				totalServe += finishedOrder->GetServTime();
				totalWait += finishedOrder->getWaitTime();

				//this switch case is to check on the type of the order alone as in some cases orders can be assigned to differnet cooks types
				switch (finishedOrder->GetType())		
				{
				case(TYPE_NRM):
					numNormOrders++;					//incrementing the number of finished normal orders
					break;
				case(TYPE_VIP):
					numVipOreders++;					//incrementing the number of finished vip orders
					break;
				case(TYPE_VGAN):
					numVganOrders++;					//incrementing the number of finished vegan orders
					break;
				}
				// the cook is injured and has to go to break we send him rest and he wasn't taken from rest
				if (c->toBreak(currentTimeStep) && c->toRest(currentTimeStep)&&!c->preparedUrgent())	
				{
					switch (c->GetType())
					{
					case TYPE_NRM:
						numberBusyNormalCooks--;			//decremting the number of busy cooks
						break;
					case TYPE_VIP:								//same procedure but for VIP
						numberBusyVipCooks--;				//decremting the number of busy cooks
						break;
					case TYPE_VGAN:							//same procedure but for VEGAN
						numberBusyVeganCooks--;					//decremting the number of busy cooks
						break;
					}
					onRestCooks.enqueue(c, -c->getExcpetedReturn());  //we enqueue him in the queue of resting cooks 
					cout << "and the preparing cook " << c->GetID() << " is sent to rest and should return at "<<c->getExcpetedReturn()<<endl;
				}
				else if (c->toBreak(currentTimeStep) && !c->toRest(currentTimeStep)) // cook should go on break and he isn't injured 
				{
					onBreakCooks.enqueue(c, -c->getExcpetedReturn());
					switch (c->GetType())
					{
					case TYPE_NRM:
						numberBusyNormalCooks--;			//decremting the number of busy cooks
						break;
					case TYPE_VIP:								//same procedure but for VIP
						numberBusyVipCooks--;				//decremting the number of busy cooks
						break;
					case TYPE_VGAN:							//same procedure but for VEGAN
						numberBusyVeganCooks--;					//decremting the number of busy cooks
						break;
					}
					cout << "and the preparing cook " << c->GetID() << " is sent to break and should return at " << c->getExcpetedReturn() << endl;
				}
						//cook isn't injured or going to break								// cook was injured and took an urgent order
				else if (((!c->toBreak(currentTimeStep) && !c->toRest(currentTimeStep)))||(c->toRest(currentTimeStep)&&c->preparedUrgent())) 			
				{
					switch (c->GetType())
					{
					case TYPE_NRM:
							availableNormalCooks.enqueue(c);  //then we enqueue him in the queue of available cooks according to his spechilaty in this case normal cooks
							numberAvailNormalCooks++;
							numberBusyNormalCooks--;			//decremting the number of busy cooks
							cout << "and the preparing cook " << c->GetID() << " is sent to normal cooks" << endl;
						break;
					case TYPE_VIP:								//same procedure but for VIP
							availableVipCooks.enqueue(c);		////then we enqueue him in the queue of available cooks according to his spechilaty in this case VIP cooks
							numberAvailVipCooks++;				//incremintg the number of available cooks type VIP
							numberBusyVipCooks--;				//decremting the number of busy cooks
							cout << "and the preparing cook " << c->GetID() << " is sent to vip cooks" << endl;
						break;
					case TYPE_VGAN:							//same procedure but for VEGAN
							availableVeganCooks.enqueue(c);			//then we enqueue him in the queue of available cooks according to his spechilaty in this case VEGAN cooks
							numberAvailVeganCooks++;				//incremintg the number of available cooks type vegan
							numberBusyVeganCooks--;					//decremting the number of busy cooks
							cout << "and the preparing cook " << c->GetID() << " is sent to vegan cooks" << endl;
						break;
					}
				}
				else if (!c->toBreak(currentTimeStep) && c->toRest(currentTimeStep)&&!c->preparedUrgent()) //cook is injured but doesn't need to go on break
				{
					switch (c->GetType())
					{
					case TYPE_NRM:
						numberBusyNormalCooks--;			//decremting the number of busy cooks
						break;
					case TYPE_VIP:								//same procedure but for VIP
						numberBusyVipCooks--;				//decremting the number of busy cooks
						break;
					case TYPE_VGAN:							//same procedure but for VEGAN
						numberBusyVeganCooks--;					//decremting the number of busy cooks
						break;
					}
					onRestCooks.enqueue(c, -c->getExcpetedReturn());  //then we enqueue him in the queue of resting cooks 
					cout << "and the preparing cook " << c->GetID() << " is sent to rest and should return at " << c->getExcpetedReturn() << endl;
				}
			}

			else	//if this order is not set to be finished on this timestep we move the order and the cooks assigned to it to the end of their respective queues
			{
				Cook* tempCook;				//here if the order finish time is not the same as the current time step then remove it from the start of the queue to its end
				Order* tempOrder;
				busyCooks.dequeue(tempCook);			//deqeuing and enqueing in both queues to keep the alignment of cooks assigned to orders
				busyCooks.enqueue(tempCook);
				servingOrders.dequeue(tempOrder);
				servingOrders.enqueue(tempOrder);
			}
		}
	}
}

void Restaurant::assigningOrders()		//This function is responisble for calling the assignorder function in every modes
{
	Order* movingOrder = nullptr;

	while (!vipOrders.isEmpty())
	{
		vipOrders.peek(movingOrder); //starting with the vip orders

		if (assignToCook(movingOrder))			//if this orders is assigned to a cook 
		{
			vipOrders.dequeue(movingOrder);			//then we dequeue it and continute to the next order
		}
		else
		{
			return;						//if the function return false then there are no available cooks of each type thats why we return
		}
	}
	while (!normalOrders.isEmpty())
	{
		normalOrders.peekFront(movingOrder);
		if (assignToCook(movingOrder))
		{
			normalOrders.dequeue(movingOrder);			//then we dequeue it and continue to the next order
		}
		else
		{
			break;				//here we break beacuse vegan cooks are not respoisnble for normal orders 
		}
	}
	while (!veganOrders.isEmpty())
	{
		veganOrders.peekFront(movingOrder);
		if (assignToCook(movingOrder))
		{
			veganOrders.dequeue(movingOrder);			//then we dequeue it and continue to the next order
		}
		else
		{
			return;
		}
	}
}




void Restaurant::checkEndBreakOrRest()
{
	Cook* restingCook;
	if (!onBreakCooks.isEmpty())
	{
		onBreakCooks.peek(restingCook);

		while (restingCook->returnToAction(currentTimeStep))// keeps checking the first entry if cook is ready to return to his available queue
		{
			onBreakCooks.dequeue(restingCook); // meaning cook ended his break and needs to be returned to the appropriate queue

			switch (restingCook->GetType()) 
			{

			case TYPE_NRM:
				numberAvailNormalCooks++;
				availableNormalCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his break and returned to nomral cooks" << endl;
				break;
			case TYPE_VIP:
				numberAvailVipCooks++;
				availableVipCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his break and returned to vip cooks" << endl;
				break;
			case TYPE_VGAN:
				numberAvailVeganCooks++;
				availableVeganCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his break and returned to vegan cooks" << endl;
				break;

			}
			

			if (onBreakCooks.isEmpty())break;
			onBreakCooks.peek(restingCook);
		}
	}
	if (!onRestCooks.isEmpty())
	{
		onRestCooks.peek(restingCook);

		while (restingCook->returnToAction(currentTimeStep))// keeps checking the first entry if cook is ready to return to his available queue
		{
			onRestCooks.dequeue(restingCook);// meaning cook ended his rest and needs to be returned to the appropriate queue
			
			switch (restingCook->GetType()) 
			{
			case TYPE_NRM:
				numberAvailNormalCooks++;
				availableNormalCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his rest and returned to nomral cooks" << endl;
				break;
			case TYPE_VIP:
				numberAvailVipCooks++;
				availableVipCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his rest and returned to vip cooks" << endl;
				break;
			case TYPE_VGAN:
				numberAvailVeganCooks++;
				availableVeganCooks.enqueue(restingCook);
				cout << "cook " << restingCook->GetID() << " ended his rest and returned to vegan cooks" << endl;
				break;
			}
			onRestCooks.peek(restingCook);
			if (onRestCooks.isEmpty()) break;// to avoid accessing null pointer
		}
	}
}

void Restaurant::Promote(int ID, double incMoney)
{
	int count = 0;
	Order** O = normalOrders.toArray(count); //converting the normal orders cook queue to an arry to find the element with the matched id
	for (int i = 0; i < count; i++)
	{

		if (O[i]->GetID() == ID)
		{
			O[i]->SetType(TYPE_VIP);
			O[i]->SetTotalMoney(O[i]->GetTotalMoney() + incMoney);
			vipOrders.enqueue(O[i], exp((O[i]->GetTotalMoney() / O[i]->GetOrdSize() * O[i]->GetArrTime())) / O[i]->GetArrTime());
			O[i]->setWaitTime(0);
			int pos = i;
			cout << "Order " << O[i]->GetID() << " is promoted to vip manually" << endl;
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

	while (!normalOrders.isEmpty()) //emptyting the queue to refill it again with changed type and money
	{
		Order* dummy;
		normalOrders.dequeue(dummy);
	}


	for (int i = 0; i < count; i++)//refilling the queue again 
	{
		normalOrders.enqueue(O[i]);
	}
}


void Restaurant::autoPromote()
{
	while (!normalOrders.isEmpty())
	{
		Order* O1;
		normalOrders.peekFront(O1);
		int c = currentTimeStep - O1->GetArrTime();

		if (c >= promoteLimit)
		{
			numAutoPromoted++;
			normalOrders.dequeue(O1);
			vipOrders.enqueue(O1, exp((O1->GetTotalMoney() / O1->GetOrdSize() * O1->GetArrTime())) / O1->GetArrTime());
			O1->setWaitTime(0);
			cout << "Order " << O1->GetID() << " is promoted to vip automatically" <<"Order must be urgent at "<<currentTimeStep+VIP_WT <<endl;

		}
		else
			break;
	}
}
void Restaurant::InteractiveMode()
{
	pGUI->PrintMessage("Choose load file name (test cases) from 1 to 6: ");
	loadFileName = pGUI->GetString();
	loadFromFile();
	currentTimeStep++;
	
	while (!(vipOrders.isEmpty() && veganOrders.isEmpty() && normalOrders.isEmpty() && servingOrders.isEmpty() && EventsQueue.isEmpty()))
	{
		cout <<"Ts:" <<currentTimeStep << endl;
		ExecuteEvents(currentTimeStep);
		Injury();
		AssignUrgentOrder();
		autoPromote();
		moveFromInservToFinished();
		checkEndBreakOrRest();
		assigningOrders();
	
		FillDrawingList();
		pGUI->waitForClick();
		currentTimeStep++;
		waitingTimeIncrementer();
	}
	
	saveToFile();
}
void Restaurant::Step_by_StepMode()
{
	pGUI->PrintMessage("Choose load file name (test cases) from 1 to 6: ");
	loadFileName = pGUI->GetString();
	loadFromFile();
	currentTimeStep++;
	waitingTimeIncrementer();
	while (!(vipOrders.isEmpty() && veganOrders.isEmpty() && normalOrders.isEmpty() && servingOrders.isEmpty() && EventsQueue.isEmpty()))
	{
		cout <<"Ts:" << currentTimeStep << endl;
		ExecuteEvents(currentTimeStep);
		Injury();
		AssignUrgentOrder();
		autoPromote();
		moveFromInservToFinished();
		checkEndBreakOrRest();
		assigningOrders();
		Sleep(1000);
		FillDrawingList();
		currentTimeStep++;
		waitingTimeIncrementer();
	}
	pGUI->waitForClick();
	
	saveToFile();
}
void Restaurant::SilentMode()
{
	pGUI->PrintMessage("Choose load file name (test cases) from 1 to 6: ");
	loadFileName = pGUI->GetString();
	loadFromFile();
	currentTimeStep++;
	
	while (!(vipOrders.isEmpty() && veganOrders.isEmpty() && normalOrders.isEmpty() && servingOrders.isEmpty() && EventsQueue.isEmpty()))
	{
		cout << "Ts:" <<currentTimeStep << endl;
		ExecuteEvents(currentTimeStep);
		Injury();
		AssignUrgentOrder();
		autoPromote();
		moveFromInservToFinished();
		checkEndBreakOrRest();
		assigningOrders();
		currentTimeStep++;
		waitingTimeIncrementer();
	}
	
	saveToFile();
}

void Restaurant::waitingTimeIncrementer()
{
	int countVip = 0, countVegan = 0, countNormal=0;

	if (!vipOrders.isEmpty())
	{
		Order** vipArray = vipOrders.toArray(countVip);
		for (int i = 0; i < countVip; i++)
		{
				vipArray[i]->setWaitTime(vipArray[i]->getWaitTime()+1);
		}
	}

	if (!veganOrders.isEmpty())
	{
		Order** veganArray = veganOrders.toArray(countVegan);
		for (int i = 0; i < countVegan; i++)
		{
			veganArray[i]->setWaitTime(currentTimeStep - veganArray[i]->GetArrTime()+1);
		}
	}

	if (!normalOrders.isEmpty())
	{
		Order** normalArray = normalOrders.toArray(countNormal);
		for (int i = 0; i < countNormal; i++)
		{
			normalArray[i]->setWaitTime(currentTimeStep - normalArray[i]->GetArrTime()+1);
		}
	}

}

 