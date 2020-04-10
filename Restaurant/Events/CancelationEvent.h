#ifndef __CANCELATION_EVENT_H_
#define __CANCELATION_EVENT_H_

#include "Event.h"


//class for the arrival event
class CancelationEvent : public Event
{
	
	int Otime;	//order time
	int ID;
	
public:
	CancelationEvent(int eTime, int ID);
	

	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif