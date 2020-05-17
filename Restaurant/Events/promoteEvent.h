#ifndef __PROMOTE_EVENT_H_
#define __PROMOTE_EVENT_H_

#include "Event.h"


class promoteEvent: public Event
{

	
	int ID;
	double incMoney;

public:
	promoteEvent(int etime,int oID, double m);


	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif