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
	excpectedReturn = 0;
	restPeriod = rest;
	preparingUrgent = false;
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

int Cook::getExcpetedReturn() const
{
	return excpectedReturn;
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

void Cook::setSpeed(float s)
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

void Cook::setfree(bool isFree)
{
	Free = isFree;
}

void Cook::setinjured(bool isInjured)
{
	Injured = isInjured;
}

void Cook::AssignOrder(Order* o, int Stime)
{
	if (CurrentOrder) return;
	CurrentOrder = o;
	Free = false;
	CurrentOrder->orderAssigned(Stime);
	double CookingTime = ceil(CurrentOrder->GetOrdSize() / speed);//calculating the time taken by the order to be finished
	int finishTime = CookingTime + Stime;                               //calculating the finish time
	CurrentOrder->SetFinishTime(finishTime);
	preparingUrgent = o->isUrgent();
	
}

bool Cook::toBreak(int timeStep)
{
	if (ordersCompleted % NumberOfDishes == 0) 
	{
		excpectedReturn = timeStep + BreakDuration;
		if (isInjured()) 
		{
			excpectedReturn = timeStep + restPeriod;
		}
		return true;
	
	}
	return false;


}

void Cook::removeOrder(int timeStep)
{
	if (!CurrentOrder) return;
	CurrentOrder->orderFinished(timeStep);
	CurrentOrder = NULL;
	ordersCompleted++;
	setfree(true);


}

bool Cook::toRest(int timeStep)
{
	if (isInjured())
	{
		excpectedReturn = timeStep+restPeriod;
		return true;
	}
	else return false;
}

bool Cook::returnToAction(int timeStep)
{
	if (timeStep == excpectedReturn) 
	{
		preparingUrgent = false;
		if (isInjured()) 
		{ 
			setinjured(false);
			setSpeed(GetSpeed() * 2);
		}
		return true; 
	}
	return false;
}

bool Cook::preparedUrgent() const
{
	return preparingUrgent;
}


