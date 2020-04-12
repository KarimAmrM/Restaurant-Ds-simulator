#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType,int size,double orderMoney):Event(eTime, oID)
{
	OrdType = oType;
	Size = size;
	OrdMoney = orderMoney;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{


	Order* pOrd = new Order(OrderID, OrdType, Size, EventTime, OrdMoney);
	pRest->addOrder(pOrd);

}
	
