#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 

	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime, waitTime;	//arrival, service start, and finish times
	int OrdSize;   //the number of dishes for the order
	int assignedToCookAt;
	bool urgent;
	bool promoted;
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int ID, ORD_TYPE r_Type, int s, int sr, double m);
	virtual ~Order();

	void SetID(int id);
	int GetID();

	void SetType(ORD_TYPE r_Type);
	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	void SetTotalMoney(double TM);
	double GetTotalMoney() const;

	void SetArrTime(int AT);
	int GetArrTime() const;

	void SetServTime(int ST);
	int GetServTime() const;

	void SetFinishTime(int FT);
	int GetFinishTime() const;

	void SetOrdSize(int OS);
	int GetOrdSize() const;

	int getWaitTime();
	void setWaitTime(int);

	void orderAssigned(int);
	void orderFinished(int);
	
	int getOrderAssignedAt();

	void setUrgent(bool);
	bool isUrgent();
	void setPromoted(bool isPromoted);
	bool isPromoted();
	
	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif