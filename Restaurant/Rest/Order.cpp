#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, int s, int ar, double m)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	OrdSize = s;
	ArrTime = ar;
	totalMoney = m;
	waitTime = 0;
	ServTime = 0;
	FinishTime = 0;
	urgent = false;
	promoted = false;
}

Order::~Order()
{
}

void Order::SetID(int id)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
}

int Order::GetID()
{
	return ID;
}

void Order::SetType(ORD_TYPE r_Type)
{
	type = r_Type;

}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d > 0 ? d : 0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::SetTotalMoney(double TM)
{
	totalMoney = TM > 0 ? TM : 0;
}
double Order::GetTotalMoney() const
{
	return totalMoney;
}

void Order::SetArrTime(int AT)
{
	ArrTime = AT;
}
int Order::GetArrTime() const
{
	return ArrTime;
}

void Order::SetServTime(int ST)
{
	ServTime = ST;
}
int Order::GetServTime() const
{
	return ServTime;
}

void Order::SetFinishTime(int FT)
{
	FinishTime = FT;
}
int Order::GetFinishTime() const
{
	return FinishTime;
}


void Order::SetOrdSize(int OS)
{
	OrdSize = OS;
}
int Order::GetOrdSize() const
{
	return OrdSize;
}

int Order::getWaitTime()
{
	return waitTime;
}

void Order::setWaitTime(int wait)
{
	waitTime = wait;
}

void Order::orderAssigned(int timeStep)
{
	assignedToCookAt = timeStep;
	setWaitTime(timeStep - ArrTime);
	setStatus(SRV);
}

void Order::orderFinished(int timeStep)
{
	SetServTime(timeStep-assignedToCookAt);
	setStatus(DONE);
}

int Order::getOrderAssignedAt()
{
	return assignedToCookAt;
}

void Order::setUrgent(bool flag)
{
	urgent =flag;
}

bool Order::isUrgent()
{
	return urgent;
}

void Order::setPromoted(bool isPromoted)
{
	promoted = isPromoted;
}

bool Order::isPromoted()
{
	return promoted;
}
