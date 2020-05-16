#include "Cook.h"
#include <math.h>

Cook::Cook(int id, ORD_TYPE r_Type, int s, int n, int bd,int rest)
{
	ID = id;
	type = r_Type;
	speed = s;
	NumberOfDishes = n;
	BreakDuration = bd;
	ordersCompleted = 0;
	Free = true;
	CurrentOrder = NULL;
	Injured = false;
	excpetedReturn = 0;
	restPeriod = rest;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int Cook::GetSpeed() const
{
	return speed;
}

int Cook::GetNumberOfDishes() const
{
	return NumberOfDishes;
}

int Cook::GetBreakDuration() const
{
	return BreakDuration;
}

bool Cook::isFree() const
{
	return Free;
}

bool Cook::isInjured() const
{
	return Injured;
}

Order* Cook::GetCurrentOrder()
{
	return CurrentOrder;
}
void Cook::setID(int id)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed = s > 0 ? s : 0;
}

void Cook::setNumberOfDishes(int n)
{
	NumberOfDishes = n;
}
void Cook::setBreakDuration(int bd)
{
	BreakDuration = bd;
}

void Cook::setisfree(bool isFree)
{
	Free = isFree;
}

void Cook::setisinjured(bool isInjured)
{
	Injured = isInjured;
}

void Cook::AssignOrder(Order* o, int Stime)
{
	if (CurrentOrder) return;
	CurrentOrder = o;
	CurrentOrder->setStatus(SRV);
	Free = false;
	CurrentOrder->SetServTime(Stime);
	double CookingTime = ceil(CurrentOrder->GetOrdSize() / double(speed));//calculating the time taken by the order to be finished
	int finishTime = CookingTime + Stime;                               //calculating the finish time
	CurrentOrder->SetFinishTime(finishTime);
	
}

bool Cook::toBreak(int timeStep)
{
	/*if (ordersCompleted % NumberOfDishes == 0 && isInjured() && isFree()) // this cook got injured in the last order before taking his break
	{
		if (BreakDuration >= restPeriod)
		{
			excpetedReturn = BreakDuration;
		}
		else
			excpetedReturn = restPeriod;
					
	}*/
	if (ordersCompleted % NumberOfDishes == 0) 
	{
		excpetedReturn = timeStep + BreakDuration;
		return true;

	}
	return false;

	



}

bool Cook::toRest(int timestep)
{
	if (isInjured() && isFree()) 
	{
		excpetedReturn = timestep + restPeriod;
		return true;
	}
	return false;
}



void Cook::removeOrder()
{
	if (!CurrentOrder) return;

	CurrentOrder = NULL;
	ordersCompleted++;
	setisfree(true);


}

bool Cook::returnToAvail(int timeStep)
{
	if (timeStep == excpetedReturn) return true;
	return false;
	
}





