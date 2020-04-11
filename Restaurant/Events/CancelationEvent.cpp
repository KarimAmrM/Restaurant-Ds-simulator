#include "cancelationEvent.h"
#include "..\Rest\Restaurant.h"


CancelationEvent::CancelationEvent(int eTime, int oID) :Event(eTime, oID)
{
	
}

void CancelationEvent::Execute(Restaurant* pRest)
{
	pRest->cancelEvent(OrderID);

	

}
