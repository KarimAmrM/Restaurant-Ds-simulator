#include "promoteEvent.h"
#include "..\Rest\Restaurant.h"



promoteEvent::promoteEvent(int etime,int oId, double incMoney) :Event(etime,oId)
{


}

void promoteEvent::Execute(Restaurant* pRest)
{
	pRest->Promote(OrderID,incMoney);



}